#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "RotaryPlatform.generated.h"

UCLASS()
class CPP_HW6_API ARotaryPlatform : public ABasePlatform
{
	GENERATED_BODY()
	
public:	
	ARotaryPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWaitFinished() override;
	virtual void InitRandomSettings() override;
	virtual void UpdatePlatform(float DeltaTime) override;

	// 회전 관련 변수
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool bIsRight;

	bool bIsWait;
};
