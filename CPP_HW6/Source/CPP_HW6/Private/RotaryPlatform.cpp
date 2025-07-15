#include "RotaryPlatform.h"

ARotaryPlatform::ARotaryPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = FRotator(0.0f, 0.0f, 180.0f);
	bIsRight = true;
	bIsWait = false;
}

void ARotaryPlatform::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &ARotaryPlatform::OnWaitFinished, WaitTime, true, WaitTime);
}

void ARotaryPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsWait)
	{
		UpdatePlatform(DeltaTime);
	}
}

void ARotaryPlatform::OnWaitFinished()
{
	if (bIsWait)
	{
		bIsWait = false;
	}
	else
	{
		bIsWait = true;
	}
}

void ARotaryPlatform::InitRandomSettings()
{
	WaitTime = FMath::RandRange(0, 5);
	// DisappearTime = FMath::RandRange(0, 10);
	TeleportTime = FMath::RandRange(0, 5);

	FRotator NewRotator;
	NewRotator.Roll = FMath::RandRange(0, 360);
	NewRotator.Pitch = FMath::RandRange(0, 360);
	NewRotator.Yaw = FMath::RandRange(0, 360);
	RotationSpeed = NewRotator;

	bIsRight = FMath::RandBool();
}

void ARotaryPlatform::UpdatePlatform(float DeltaTime)
{
	AddActorLocalRotation(RotationSpeed * DeltaTime * (bIsRight ? 1 : -1));
}