// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMyActor::AMyActor() : eventCount(0), movingDistance(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube'"));

	if (cubeMesh.Succeeded())
	{
		staticMesh->SetStaticMesh(cubeMesh.Object);
	}

	startLocation = FVector(0.0f, 0.0f, 0.0f);
	currentLocation = startLocation;
	SetActorLocation(currentLocation);
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	Move();
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMyActor::distance(FVector v1, FVector v2)
{
	float dx = v1.X - v2.X;
	float dy = v1.Y - v2.Y;
	return FMath::Sqrt(dx * dx + dy * dy);
}

int32 AMyActor::Step()
{
	return FMath::RandRange(0, 1);
}
// 출력 로그 요소 : 스탭마다 이동거리, 좌표
// 이동 후 총 이동거리와 이벤트 발생 횟수
void AMyActor::Move()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Position: (%.0f, %.0f, %.0f)"), startLocation.X, startLocation.Y, startLocation.Z);

	for (int i = 0; i < 10; i++)
	{
		TriggerEventWithProbability(20.00);
		FVector tmpLocation = currentLocation;
		currentLocation.X += Step();
		currentLocation.Y += Step();

		movingDistance += distance(tmpLocation, currentLocation);

		UE_LOG(LogTemp, Log, TEXT("Current Position: (%.0f, %.0f, %.0f)"), currentLocation.X, currentLocation.Y, currentLocation.Z);
		UE_LOG(LogTemp, Log, TEXT("Moving Distance: %f"), distance(tmpLocation, currentLocation));
	}

	UE_LOG(LogTemp, Log, TEXT("Total Distance: %f"), movingDistance);
	UE_LOG(LogTemp, Log, TEXT("Number of events: %d"), eventCount);
}

void AMyActor::TriggerEventWithProbability(float probability)
{
	int32 RandomValue = FMath::RandRange(1, 100);
	if (RandomValue <= probability)
	{
		UE_LOG(LogTemp, Log, TEXT("Event Triggered!"));
		eventCount++;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Event Not Triggerd"));
	}
}

int32 AMyActor::createEvent()
{
	return int32();
}
