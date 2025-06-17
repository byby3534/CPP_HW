// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class CPP_HW5_API AMyActor : public AActor
{
	GENERATED_BODY()
private:
	int32 eventCount;
	float movingDistance;

public:	
	UPROPERTY(VisibleAnywhere, Category = MyValue)
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(VisibleAnywhere, Category = MyValue)
	FVector startLocation;

	UPROPERTY(VisibleAnywhere, Category = MyValue)
	FVector currentLocation;



	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Move();
	void TriggerEventWithProbability(float probability);

	float distance(FVector v1, FVector v2);
	int32 Step();
	int32 createEvent();

};
