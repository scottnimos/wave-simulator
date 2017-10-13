/* This file is an Actor Component which is added on to a actor (static mesh)
with he intent to interact and float on a waveworks water surface
it exposes an api to ue4 to add test points and change the amount of buoyancy force
Also playing with linear and angular dampening has some interesting effects to make it realistic.

Author: Scott Nimos, Nathan Mangan
*/

#pragma once

#include "Components/ActorComponent.h"
#include "BuoyancyManager.generated.h"


class UBuoyancyManager;

USTRUCT(BlueprintType)
struct FWavePointData
{
	GENERATED_BODY()
		UPROPERTY()
		bool bIsUnder;
	UPROPERTY()
		float Amount;
	UPROPERTY()
		float Delta;
	UPROPERTY()
		FVector WaveIntersectPoint;
	UPROPERTY()
		float WaveHeight;
	//Constructor
	FWavePointData()
	{
		bIsUnder = false;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAVEWORKSTESTER_API UBuoyancyManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuoyancyManager();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnRecievedWaveWorksDisplacement(TArray<FVector4> OutDisplacements);

private:
	void CalculateForceToAdd();
	FWavePointData ProcessWaveHeightAtPoint(FVector pointLocation, const FTransform& actorTransform);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveWorks)
		AActor* WaveWorksActor;
	UPROPERTY(Category = "WaveWorks", EditAnywhere, BlueprintReadWrite)
		TArray<FVector> WaterTestPoints;
	UPROPERTY(Category = "WaveWorks", EditAnywhere, BlueprintReadWrite)
		float PointThickness;
	UPROPERTY(Category = "WaveWorks", EditAnywhere, BlueprintReadWrite)
		float DisplacementRatio;
	UPROPERTY(Category = "WaveWorks", EditAnywhere, BlueprintReadWrite)
		float MassMultiplier;
	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
		bool bDrawDebugSpheres;
private:
	class UWaveWorksComponent* WaveWorksComponent;
	FVector4 WaveWorksOutDisplacement;
	FVector InitialPosition;
	UStaticMeshComponent* staticMesh;
	FVectorArrayDelegate WaveWorksRecieveDisplacementDelegate;
};
