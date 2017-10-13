// This file is provided from NVDIA showing demonstation of a raycast.. 

#include "WaveworksTester.h"
#include "RaycastOceanTutorial.h"


// Sets default values
ARaycastOceanTutorial::ARaycastOceanTutorial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaveWorksActor = nullptr;
	RaycastOriginActor = nullptr;
	WaveWorksComponent = nullptr;
	WaveWorksRaycastResultDelegate = nullptr;
}

// Called when the game starts or when spawned
void ARaycastOceanTutorial::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	UActorComponent* pComponent = WaveWorksActor->GetComponentByClass(UWaveWorksComponent::StaticClass());
	WaveWorksComponent = Cast<UWaveWorksComponent>(pComponent);

	WaveWorksRaycastResultDelegate = FWaveWorksRaycastResultDelegate::CreateUObject(this, &ARaycastOceanTutorial::OnRecievedWaveWorksIntersectPoints);
}

// Called every frame
void ARaycastOceanTutorial::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (WaveWorksComponent != nullptr && RaycastOriginActor != nullptr)
	{
		FVector OriginPoint = RaycastOriginActor->GetActorLocation();
		FVector RayDirection(1.0, 1.0, -1.0);
		RayDirection.Normalize();

		WaveWorksComponent->GetIntersectPointWithRay(OriginPoint / 100.0, RayDirection, WaveWorksRaycastResultDelegate);

		// draw line
		{
			DrawDebugLine(
				GetWorld(),
				OriginPoint,
				IntersectPoint,
				FColor(255, 0, 0),
				false, -1, 0,
				12.333
				);

			DrawDebugSphere(
				GetWorld(),
				IntersectPoint,
				60,
				32,
				FColor(0, 255, 0)
				);
		}
	}
}

void ARaycastOceanTutorial::OnRecievedWaveWorksIntersectPoints(FVector OutIntersectPoint, bool bSuc)
{
	IntersectPoint = OutIntersectPoint * 100.0;
}
