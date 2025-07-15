#include "MySpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include <BasePlatform.h>

AMySpawner::AMySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	NumPlatforms = 0;
	LocationZ = 0.0f;
}

void AMySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnMyActor(PlatformClass_A);
}

void AMySpawner::SpawnMyActor(TSubclassOf<AActor> PlatformClass)
{
	if (!PlatformClass)
	{
		return;
	}

	for (int32 i = 0; i < NumPlatforms; ++i)
	{
		//// 임의 위치 생성
		//FVector SpawnLocation = GetActorLocation() + FVector(
		//	FMath::FRandRange(-1000.0f, 1000.0f),
		//	FMath::FRandRange(-1000.0f, 1000.0f),
		//	LocationZ
		//);

		FVector Center(0, 0, 0);
		FVector Extent(1500, 1500, 0); // Z축은 고정
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Center, Extent);
		SpawnLocation.Z = LocationZ;

		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(PlatformClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (ABasePlatform* Platform = Cast<ABasePlatform>(SpawnedActor))
		{
			Platform->InitRandomSettings();
		}
	}
}
