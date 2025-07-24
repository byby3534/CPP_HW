#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable)
	{
		return nullptr;
	}

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
	{
		return nullptr;
	}

	float TotalChance = 0.0f;
	// 이부분을 수정해야하네 웨이브별로 토탈찬스가 합쳐지게
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass)
	{
		return nullptr;
	}
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClass, GetRandomPointInValue(), FRotator::ZeroRotator);

	return SpawnedActor;
}

FVector ASpawnVolume::GetRandomPointInValue() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent(); // 박스의 크기를 반환함 중심기준으로 X Y Z의 거리를 반환함
	FVector BoxOrigin = SpawningBox->GetComponentLocation(); // 원점 위치 반환
	return BoxOrigin + FVector(FMath::FRandRange(-BoxExtent.X, BoxExtent.X), FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y), FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}
