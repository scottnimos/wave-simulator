// Fill out your copyright notice in the Description page of Project Settings.

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
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBuoyancyManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UActorComponent* pComponent = WaveWorksActor->GetComponentByClass(UWaveWorksComponent::StaticClass());
	WaveWorksComponent = Cast<UWaveWorksComponent>(pComponent);
	InitialPosition = GetOwner()->GetActorLocation();

	WaveWorksRecieveDisplacementDelegate = FVectorArrayDelegate::CreateUObject(this, &UBuoyancyManager::OnRecievedWaveWorksDisplacement);
	
}


// Called every frame
void UBuoyancyManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	staticMesh = components[i];
	}*/

	if (!staticMesh)
	{
		GLog->Log("Root component is not a static mesh");
		return;
	}

	staticMesh->AddForce(this->CalculateForceToAdd()); //we will want the mass to the boat, not the sphere in future														
}

void UBuoyancyManager::OnRecievedWaveWorksDisplacement(TArray<FVector4> OutDisplacements)
{
	if (OutDisplacements.Num() > 0)
	{
		WaveWorksOutDisplacement = OutDisplacements[0];
	}
}

FVector UBuoyancyManager::CalculateForceToAdd()
{
	float currentActorPositionZ = GetOwner()->GetActorLocation().Z;
	float waveActorPositionZ = WaveWorksOutDisplacement.Z * 100.0f + WaveWorksComponent->SeaLevel;
	float sphereRadius = 50.0f; //TODO change this later
	float densityOcean = 1000.0f;
	float gravity = 1000;

	FVector buoyancyForce = FVector(0,0,-2000);
	if (currentActorPositionZ - waveActorPositionZ < sphereRadius)
	{
		//first calculate h
		float h;
		if (currentActorPositionZ - waveActorPositionZ < 0)
		{
			h = sphereRadius - (currentActorPositionZ - waveActorPositionZ);
		}
		else if (currentActorPositionZ - waveActorPositionZ > 0)
		{
			h = sphereRadius + (currentActorPositionZ - waveActorPositionZ);
		}
		else 
		{
			h = sphereRadius;
		}
		h = h / 100.0f;
		
		float volume = ((PI * h * (3 * sphereRadius - h)) / 3)/10;
		float forceapp = (volume * densityOcean * gravity)/100.0f;
		GLog->Log(FString::SanitizeFloat(forceapp));
		buoyancyForce = FVector(0, 0, forceapp);

	}
	return buoyancyForce;
}

