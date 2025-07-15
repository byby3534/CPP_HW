#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "MovingPlatform.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UENUM()
enum class EPlatformState : uint8
{
	Moving			UMETA(DisplayName = "Moving"),
	Rotating		UMETA(DisplayName = "Rotary"),
	MoveAndRotate	UMETA(DisplayNmae = "Move And Rotate"),
	Waiting			UMETA(DisplayName = "Wating"),
	Count
};

UCLASS()
class CPP_HW6_API AMovingPlatform : public ABasePlatform
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 이동 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Move")
	FVector StartLocation;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Move")
	FVector ActorDirection;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MaxRange;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	float CurrentMovingDistance;

	// 회전 관련 변수
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool bIsRight;

	UPROPERTY(EditAnywhere)
	EPlatformState State;

	virtual void UpdatePlatform(float DeltaTime) override;
	virtual void OnWaitFinished() override;
	virtual void InitRandomSettings() override;

	void MovePlatform(float DeltaTime);
	void RotatePlatform(float DeltaTime);

	float Distance(const FVector& Start, const FVector& End) const;
private:
	EPlatformState PrevState;
};
