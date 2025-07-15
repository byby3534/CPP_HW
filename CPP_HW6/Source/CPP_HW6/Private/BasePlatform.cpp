#include "BasePlatform.h"
#include "TimerManager.h"

ABasePlatform::ABasePlatform()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetWorldLocation(FVector(0, 0, 0));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DisappearTime = 0.0f;
	TeleportTime = 0.0f;
}

void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(DisapearTimerHandle, this, &ABasePlatform::DisappearPlatform, DisappearTime, true, DisappearTime);
	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &ABasePlatform::TeleportPlatform, TeleportTime, true, TeleportTime);
}

void ABasePlatform::DisappearPlatform()
{
	// Mesh->SetSimulatePhysics(true);
	if (!IsHidden())
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}
}

void ABasePlatform::TeleportPlatform()
{
	// 난수로 RandomLocation을 생성하여 주기적으로 이동
	FVector NewLocation;
	NewLocation.X = FMath::RandRange(-500, 500);
	NewLocation.Y = FMath::RandRange(-500, 500);
	NewLocation.Z = FMath::RandRange(100, 300);
	SetActorLocation(NewLocation);
}

void ABasePlatform::TimerReset()
{
	GetWorld()->GetTimerManager().ClearTimer(DisapearTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DisapearTimerHandle, this, &ABasePlatform::DisappearPlatform, DisappearTime, true, DisappearTime);

	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &ABasePlatform::TeleportPlatform, TeleportTime, true, TeleportTime);
}
