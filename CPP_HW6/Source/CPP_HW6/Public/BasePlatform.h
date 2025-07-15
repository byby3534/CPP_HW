// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlatform.generated.h"

UCLASS(Abstract)
class CPP_HW6_API ABasePlatform : public AActor
{
	GENERATED_BODY()
	
public:
	ABasePlatform();

	virtual void InitRandomSettings() {}

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh")
	UStaticMeshComponent* Mesh;

	void DisappearPlatform();

	void TeleportPlatform();

	void TimerReset();

	virtual void OnWaitFinished() {}

	virtual void UpdatePlatform(float DeltaTime) PURE_VIRTUAL(ABasePlatform::UpdatePlatform, ;);

	UPROPERTY(EditAnywhere, Category = "Timer")
	float DisappearTime;
	UPROPERTY(EditAnywhere, Category = "Timer")
	float TeleportTime;
	UPROPERTY(EditAnywhere, Category = "Timer")
	float WaitTime;

	// 타이머 핸들
	FTimerHandle WaitTimerHandle;
	FTimerHandle DisapearTimerHandle;
	FTimerHandle TeleportTimerHandle;
};
