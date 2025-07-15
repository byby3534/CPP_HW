// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorDirection = GetActorForwardVector();
	MoveSpeed = 100.0f;
	MaxRange = 500.0f;
	CurrentMovingDistance = 0.0f;

	RotationSpeed = FRotator(0.0f, 0.0f, 0.0f);
	bIsRight = true;

	WaitTime = 0.01f; // 0.0fÀ¸·Î ÇÏ¸é SetTimer()ÇÔ¼ö ¾ÃÈû

	State = EPlatformState::Moving;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + (ActorDirection * MaxRange);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePlatform(DeltaTime);
}

void AMovingPlatform::UpdatePlatform(float DeltaTime)
{
	// FSM
	switch (State)
	{
	case EPlatformState::Moving:
		MovePlatform(DeltaTime);
		break;
	case EPlatformState::Rotating:
		RotatePlatform(DeltaTime);
		break;
	case EPlatformState::MoveAndRotate:
		MovePlatform(DeltaTime);
		RotatePlatform(DeltaTime);
		break;
	case EPlatformState::Waiting:
		break;
	}
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += ActorDirection * MoveSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);

	CurrentMovingDistance = Distance(StartLocation, CurrentLocation);
	if (Distance(TargetLocation, CurrentLocation) < 10.0f)
	{
		MoveSpeed *= -1;
		if (TargetLocation == StartLocation)
		{
			TargetLocation = StartLocation + (ActorDirection * MaxRange);
			CurrentMovingDistance = 0.0f;
		}
		else
		{
			TargetLocation = StartLocation;
			CurrentMovingDistance = 0.0f;
		}
		PrevState = State;
		State = EPlatformState::Waiting;
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AMovingPlatform::OnWaitFinished, WaitTime);
	}
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(RotationSpeed * DeltaTime * (bIsRight ? 1 : -1));
}

void AMovingPlatform::OnWaitFinished()
{
	State = PrevState;
}

void AMovingPlatform::InitRandomSettings()  
{  
    WaitTime = FMath::RandRange(0, 10);  
    // DisappearTime = FMath::RandRange(0, 10);  
    // TeleportTime = FMath::RandRange(0, 10);  

    int32 Count = static_cast<int32>(EPlatformState::Count);  
	int32 RandomIndex = FMath::RandRange(0, Count - 1);
	State = static_cast<EPlatformState>(RandomIndex);

	/*FRotator NewRotator;
	NewRotator.Roll = FMath::RandRange(0, 360);
	NewRotator.Pitch = FMath::RandRange(0, 360);
	NewRotator.Yaw = FMath::RandRange(0, 360);
	RotationSpeed = NewRotator;

	bIsRight = FMath::RandBool();*/

	MaxRange = FMath::FRandRange(200.0f, 500.0f);
	MoveSpeed = FMath::FRandRange(50.0f, 600.0f);

	FVector NewDirection;
	NewDirection.X = FMath::RandRange(0, 1);
	NewDirection.Y = FMath::RandRange(0, 1);
	NewDirection.Z = FMath::RandRange(0, 1);
	ActorDirection = NewDirection;

	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + (ActorDirection * MaxRange);

	TimerReset();
}

float AMovingPlatform::Distance(const FVector& Start, const FVector& End) const
{
	FVector Delta = Start - End;
	return FMath::Sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
}