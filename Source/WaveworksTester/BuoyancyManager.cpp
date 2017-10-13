/* This file is an Actor Component which is added on to a actor (static mesh)
with he intent to interact and float on a waveworks water surface
it exposes an api to ue4 to add test points and change the amount of buoyancy force
Also playing with linear and angular dampening has some interesting effects to make it realistic.

Author: Scott Nimos, Nathan Mangan
*/

#include "WaveworksTester.h"
#include "BuoyancyManager.h"


// Sets default values for this component's properties
UBuoyancyManager::UBuoyancyManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	WaterTestPoints.Emplace(FVector(-100, 0, 0));
	WaterTestPoints.Emplace(FVector(100, 0, 0));
	WaterTestPoints.Emplace(FVector(0, -100, 0));
	WaterTestPoints.Emplace(FVector(0, 100, 0));
	WaterTestPoints.Emplace(FVector(0, 0, -40));

	PointThickness = 10.0f;
	DisplacementRatio = 1.0f;
	MassMultiplier = 1.0f;

}


// Called when the game starts
void UBuoyancyManager::BeginPlay()
{
	Super::BeginPlay();

	//check if waveworks actor been selected if not notify user
	if (WaveWorksActor == NULL) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "WaveWorks Actor Not Specified On Buoyancy Manager!");
		return;
	}

	// ...
	UActorComponent* pComponent = WaveWorksActor->GetComponentByClass(UWaveWorksComponent::StaticClass());
	WaveWorksComponent = Cast<UWaveWorksComponent>(pComponent);
	InitialPosition = GetOwner()->GetActorLocation();

	WaveWorksRecieveDisplacementDelegate = FVectorArrayDelegate::CreateUObject(this, &UBuoyancyManager::OnRecievedWaveWorksDisplacement);

	// Get All static Mesh Components - here ive made a for loop to grab all of them for multiple test points in the future
	//UStaticMeshComponent* sm = 0;//have to set to 0 (Ue4 or c++ crap) same as null
	TArray<UStaticMeshComponent*> components;
	GetOwner()->GetComponents<UStaticMeshComponent>(components);//pointers are confusing
	staticMesh = components[0];
	/*for (int32 i = 0; i<components.Num(); i++) //Count is zero
	{
	staticMesh = components[i];
	}*/

	if (!staticMesh)
	{
		GLog->Log("Root component is not a static mesh");
		return;
	}
}

// Called every frame
void UBuoyancyManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Make sure waveworks been selected
	if (WaveWorksActor == NULL) {
		return;
	}

	/*TArray<FVector2D> samplePoints;
	FVector2D samplePos(InitialPosition.X / 100.0f, InitialPosition.Y / 100.0f);
	samplePoints.Add(samplePos);
	WaveWorksComponent->SampleDisplacements(samplePoints, WaveWorksRecieveDisplacementDelegate);*/


	CalculateForceToAdd();
	//make sure gravity is on
	if (!staticMesh->IsGravityEnabled())
	{
		//staticMesh->SetEnableGravity(true);
	}													
}

void UBuoyancyManager::OnRecievedWaveWorksDisplacement(TArray<FVector4> OutDisplacements)
{
	if (OutDisplacements.Num() > 0)
	{
		WaveWorksOutDisplacement = OutDisplacements[0];
	}
}

void UBuoyancyManager::CalculateForceToAdd()
{
	FTransform actorTransform = GetOwner()->GetTransform();

	// Buoyancy effects
	float massOfPoint = (staticMesh->GetMass() / WaterTestPoints.Num());

	for (int32 i = 0; i < WaterTestPoints.Num(); i++)
	{
		FVector currentPoint = WaterTestPoints[i];
		FVector transformedPoint = actorTransform.TransformPosition(currentPoint);
		FWavePointData waveData = ProcessWaveHeightAtPoint(currentPoint, actorTransform);

		FVector gravityAmount;
		if (waveData.bIsUnder)
		{
			FVector gravityVector = FVector(0.0f, 0.0f, 980.0f) * DisplacementRatio;
			gravityAmount = gravityVector * massOfPoint * waveData.Amount;
		}
		else
		{
			FVector gravityVector = FVector(0.0f, 0.0f, -980.0f);
			gravityAmount = gravityVector * waveData.Amount * massOfPoint * MassMultiplier;
		}
		staticMesh->AddForceAtLocation(gravityAmount, transformedPoint);

		if (bDrawDebugSpheres)
		{
			DrawDebugSphere(GetWorld(), transformedPoint, PointThickness, 32, FColor(0, 255, 0));
			DrawDebugLine(GetWorld(), transformedPoint, transformedPoint + gravityAmount, FColor(0, 255, 0));
		}
	}
}

FWavePointData UBuoyancyManager::ProcessWaveHeightAtPoint(FVector pointLocation, const FTransform& actorTransform)
{
	FWavePointData waveData;
	FVector transformedPoint = actorTransform.TransformPosition(pointLocation);

	TArray<FVector2D> samplePoints;
	FVector2D samplePos(pointLocation.X / 100.0f, pointLocation.Y / 100.0f);
	samplePoints.Add(samplePos);
	WaveWorksComponent->SampleDisplacements(samplePoints, WaveWorksRecieveDisplacementDelegate);

	//FVector waveHeight = RiverManager->GetWaveHeightValue(transformedPoint, GetWorld()->GetTimeSeconds());
	waveData.WaveHeight = WaveWorksOutDisplacement.Z * 100.0f + WaveWorksComponent->SeaLevel;

	float heightDifference = waveData.WaveHeight - transformedPoint.Z;
	waveData.Delta = heightDifference;
	waveData.bIsUnder = heightDifference > 0.0f;
	float displacementAmount = FMath::Abs(FMath::Clamp(FMath::Pow(heightDifference / PointThickness, 3.0f), -1.0f, 1.0f));
	waveData.Amount = displacementAmount;

	FVector waveHeightSum = transformedPoint + waveData.WaveHeight;
	waveData.WaveIntersectPoint = FVector(waveHeightSum.X, waveHeightSum.Y, transformedPoint.Z);

	return waveData;
}


