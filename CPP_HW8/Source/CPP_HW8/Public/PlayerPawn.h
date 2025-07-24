#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputDataAsset;
class UInputMappingContext;
class UWidgetComponent;

struct FInputActionValue;

UCLASS()
class CPP_HW8_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UArrowComponent> ArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 CurrentHealth;

	// DataAsset(IMC, IA)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputData")
	TObjectPtr<UInputDataAsset> InputDataAsset;

	// Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed;
	float SprintSpeed;

	// Look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float LookSpeed;

	// Move based on camera direction?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveRelativeToCamera;

	// Falling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	bool bIsFalling;

	// Gravity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GravityMovement")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GravityMovement")
	float JumpForce;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GravityMovement")
	float AirControl;
	UPROPERTY(BlueprintReadOnly, Category = "GravityMovement")
	bool bIsGround;

	// Current Input Value
	FVector2D InputValue;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Damaged
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	void OnDeath();
	void UpdateOverheadHP();

	// Binding Function
	void Move(const FInputActionValue& Value);
	void MoveStop(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);
};
