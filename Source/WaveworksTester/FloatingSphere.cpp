// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveworksTester.h"
#include "FloatingSphere.h"
// Sets default values for this component's properties
UFloatingSphere::UFloatingSphere()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFloatingSphere::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UActorComponent* pComponent = WaveWorksActor->GetComponentByClass(UWaveWorksComponent::StaticClass());
	WaveWorksComponent = Cast<UWaveWorksComponent>(pComponent);
	InitialPosition = GetOwner()->GetActorLocation();

	WaveWorksRecieveDisplacementDelegate = FVectorArrayDelegate::CreateUObject(this, &UFloatingSphere::OnRecievedWaveWorksDisplacement);
}

DECLARE_DELEGATE_OneParam(FStringDelegate, FString);
DECLARE_DELEGATE_OneParam(FArrayDelegate, TArray<FVector4>);

// Called every frame
void UFloatingSphere::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	TArray<FVector2D> samplePoints;
	FVector2D samplePos(InitialPosition.X / 100.0f, InitialPosition.Y / 100.0f);
	samplePoints.Add(samplePos);
	
	WaveWorksComponent->SampleDisplacements(samplePoints, WaveWorksRecieveDisplacementDelegate);
	
	// Get All static Mesh Components - here ive made a for loop to grab all of them for multiple test points in the future
	//UStaticMeshComponent* sm = 0;//have to set to 0 (Ue4 or c++ crap) same as null
	TArray<UStaticMeshComponent*> components;
	GetOwner()->GetComponents<UStaticMeshComponent>(components);//pointers are confusing
	UStaticMeshComponent* staticMesh = components[0];
	/*for (int32 i = 0; i<components.Num(); i++) //Count is zero
	{
		staticMesh = Components[i];
	}*/

	if (!staticMesh)
	{
		GLog->Log("Root component is not a static mesh");
		return;
	}

	staticMesh->AddForce(this->CalculateForceToAdd() * staticMesh->GetMass()); //we will want the mass to the boat, not the sphere in future

	/*
	//Set the Actor Position to specific point on the wave
	FVector newActorPosition;
	newActorPosition.X = WaveWorksOutDisplacement.X * 100.0f + InitialPosition.X;
	newActorPosition.Y = WaveWorksOutDisplacement.Y * 100.0f + InitialPosition.Y;
	newActorPosition.Z = WaveWorksOutDisplacement.Z * 100.0f + WaveWorksComponent->SeaLevel;
	GetOwner()->SetActorLocation(newActorPosition);
	*/
}

void UFloatingSphere::OnRecievedWaveWorksDisplacement(TArray<FVector4> OutDisplacements)
{
	if (OutDisplacements.Num() > 0)
	{
		WaveWorksOutDisplacement = OutDisplacements[0];
	}
}

FVector UFloatingSphere::CalculateForceToAdd()
{
	return FVector(0, 0, 1000);
}