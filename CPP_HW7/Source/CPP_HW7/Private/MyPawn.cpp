// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "PlayerInputData.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->SetCapsuleRadius(93);
	Capsule->SetSimulatePhysics(false);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	SpringArm->bUsePawnControlRotation = false;

	MoveSpeed = 300.0f;
	RotationSpeed = 1.5f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->PlayerInputData->InputMove)
			{
				EnhancedInput->BindAction(
					PlayerController->PlayerInputData->InputMove,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}

			if (PlayerController->PlayerInputData->InputLook)
			{
				EnhancedInput->BindAction(
					PlayerController->PlayerInputData->InputLook,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}

void AMyPawn::Move(const FInputActionValue& value)
{
	FVector Input = value.Get<FVector>();
	if (Input.IsNearlyZero()) return;

	FVector MoveDir = GetActorForwardVector() * Input.X + GetActorRightVector() * Input.Y + GetActorUpVector() * Input.Z;
	MoveDir.Normalize();

	AddActorWorldOffset(MoveDir * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void AMyPawn::Look(const FInputActionValue& value)
{
	FVector Input = value.Get<FVector>();
	if (Input.IsNearlyZero()) return;

	float YawInput = Input.X * RotationSpeed;
	float PitchInput = Input.Y * RotationSpeed;
	float RollInput = Input.Z * RotationSpeed;

	AddActorLocalRotation(FRotator(PitchInput, YawInput, RollInput), true);

	//// 카메라 Pitch 처리
	//ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch - PitchInput, -80.f, 80.f);
	//SpringArm->SetRelativeRotation(FRotator(ControlRotation.Pitch, 0.f, 0.f));
}

