// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ThirdPersonPlayerController.h"
#include "MainGameState.h"
#include "InputDataAsset.h"
#include "Components/TextBlock.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init Components
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCylinder"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleComponent->SetSimulatePhysics(false);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(CapsuleComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh->SetupAttachment(CapsuleComponent);
	Mesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CapsuleComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(Mesh);
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// Move
	Velocity = FVector::ZeroVector;
	NormalSpeed = 300.0f;
	MoveSpeed = NormalSpeed;
	SprintSpeedMultiplier = 2.0f;
	SprintSpeed = MoveSpeed * SprintSpeedMultiplier;
	
	// Look
	LookSpeed = 100.0f;
	
	// Gravity
	Gravity = -980.0f;
	JumpForce = 600.0f;
	AirControl = 0.3f;
	bIsGround = true;

	MaxHealth = 50;
	CurrentHealth = MaxHealth;
}

float APlayerPawn::GetHealth() const
{
	return CurrentHealth;
}

void APlayerPawn::AddHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	// AddMappingContext
	// GetController는 AController타입으로 반환 됨 그래서 AController를 상속받아 사용자가 만든 컨트롤러로 다운 캐스팅함
	// 만약 Controller내부에서 가져와서 매핑하는경우 캐스팅 필요없음
	if (AThirdPersonPlayerController* PlayerController = Cast<AThirdPersonPlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) // 플레이어 컨트롤러에서 로컬플레이어 가져오는 함수 호출
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // 로컬플레이어의 서브시스템 가져오기
			{
				if (InputDataAsset && InputDataAsset->DefaultMappingContext) // 데이터 에셋과 데이터 IsValid?
				{
					InputSystem->AddMappingContext(InputDataAsset->DefaultMappingContext, 0);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("InputDataAsset is missing or not set up properly."));
				}
			}
		}
	}

	if (!bMoveRelativeToCamera)
	{
		SpringArm->bInheritYaw = true;
	}
	UpdateOverheadHP();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 지면 감지
	FHitResult HitResult; // 충돌 처리
	FVector StartTrace = GetActorLocation(); // 라인트레이스의 시작 위치

	FVector EndTrace = StartTrace + FVector(0.0f, 0.0f, -(CapsuleComponent->GetScaledCapsuleHalfHeight() + 10.0f));
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 자기 자신 제외

	bIsGround = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionParams);

	/*DrawDebugLine(
		GetWorld(),
		StartTrace,
		EndTrace,
		bIsGround ? FColor::Green : FColor::Red,
		false,
		0.0f,
		0,
		1.0f
	);*/

	if (!bIsGround)
	{
		Velocity.Z += Gravity * DeltaTime;
	}

	// 착지처리
	if (bIsGround)
	{
		bIsFalling = false;
		if (Velocity.Z < 0.0f)
		{
			Velocity.Z = 0.0f;
			// SetActorLocation(HitResult.ImpactPoint + FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight())); // 지면에 서 있도록 처리
		}
	}
	else
	{
		bIsFalling = true;
	}

	// 최종 중력 및 이동 적용
	// 중력 반영 포함한 전체 속도
	FVector WorldVelocity = FVector(Velocity.X, Velocity.Y, Velocity.Z);
	FVector MovementDelta = WorldVelocity * DeltaTime;

	FHitResult MoveHit;
	AddActorWorldOffset(MovementDelta, true, &MoveHit);

	// 충돌 시 슬라이딩 처리
	if (MoveHit.bBlockingHit)
	{
		// 슬라이딩 처리
		FVector SlideDelta = FVector::VectorPlaneProject(MovementDelta, MoveHit.Normal);
		AddActorWorldOffset(SlideDelta, true);

		// 디버그 표시
		// DrawDebugSphere(GetWorld(), MoveHit.ImpactPoint, 10, 12, FColor::Blue);
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA_MOVE
		if (InputDataAsset && InputDataAsset->MoveAction)
		{
			Input->BindAction(
				InputDataAsset->MoveAction,
				ETriggerEvent::Triggered,
				this,
				&APlayerPawn::Move
			);
		}

		if (InputDataAsset && InputDataAsset->MoveAction)
		{
			Input->BindAction(
				InputDataAsset->MoveAction,
				ETriggerEvent::Completed,
				this,
				&APlayerPawn::MoveStop
			);
		}

		// IA_LOOK
		if (InputDataAsset && InputDataAsset->LookAction)
		{
			Input->BindAction(
				InputDataAsset->LookAction,
				ETriggerEvent::Triggered,
				this,
				&APlayerPawn::Look
			);
		}

		// IA_JUMP
		if (InputDataAsset && InputDataAsset->LookAction)
		{
			Input->BindAction(
				InputDataAsset->JumpAction,
				ETriggerEvent::Triggered,
				this,
				&APlayerPawn::Jump
			);
		}

		// IA_SPRINT
		if (InputDataAsset && InputDataAsset->SprintAction)
		{
			Input->BindAction(
				InputDataAsset->SprintAction,
				ETriggerEvent::Triggered,
				this,
				&APlayerPawn::SprintStart
			);
		}

		if (InputDataAsset && InputDataAsset->SprintAction)
		{
			Input->BindAction(
				InputDataAsset->SprintAction,
				ETriggerEvent::Completed,
				this,
				&APlayerPawn::SprintStop
			);
		}
	}
}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	//UE_LOG(LogTemp, Warning, TEXT("Damaged! Health : %d"), CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void APlayerPawn::OnDeath()
{
	AMainGameState* MainGameState = Cast<AMainGameState>(GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr);
	if (MainGameState)
	{
		MainGameState->OnGameOver(true);
	}
}

void APlayerPawn::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHealth, MaxHealth)));
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	InputValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("Input Move"));

	if (InputValue.IsNearlyZero()) return;

	// 회전 타겟 로테이션 초기화

	if (bMoveRelativeToCamera)
	{
		// 회전
		FRotator TargetYawRotation(0.0f, Camera->GetComponentRotation().Yaw, 0.0f);
		// InputValue(1, 0)  정면은 처리 X
		if (InputValue.X == 1.0f && InputValue.Y == -1.0f) // InputValue(1, -1) 전방 왼쪽 45도
		{
			TargetYawRotation.Yaw -= 45.0f;
		}
		else if (InputValue.X == 1.0f && InputValue.Y == 1.0f) // InputValue(1, 1) 전방 오른쪽 45도
		{
			TargetYawRotation.Yaw += 45.0f;
		}
		else if (InputValue.X == 0.0f && InputValue.Y == -1.0f) // InputValue(0, -1) 왼쪽 90도
		{
			TargetYawRotation.Yaw -= 90.0f;
		}
		else if (InputValue.X == 0.0f && InputValue.Y == 1.0f) // InputValue(0, 1) 오른쪽 90도
		{
			TargetYawRotation.Yaw += 90.0f;
		}
		else if (InputValue.X == -1.0f && InputValue.Y == -1.0f) // InputValue(-1, -1) 후방 왼쪽 135도
		{
			TargetYawRotation.Yaw -= 135.0f;
		}
		else if (InputValue.X == -1.0f && InputValue.Y == 1.0f) // InputValue(-1, -1) 후방 오른쪽 135도
		{
			TargetYawRotation.Yaw += 135.0f;
		}
		else if (InputValue.X == -1.0f && InputValue.Y == 0.0f) // InputValue(-1, -1) 후방
		{
			TargetYawRotation.Yaw -= 180.0f;
		}
		FRotator CurrentRot = GetActorRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetYawRotation, GetWorld()->GetDeltaSeconds(), 10.0f);

		SetActorRotation(NewRot);
	}

	// 이동
	FRotator CameraRotation = Camera->GetComponentRotation();
	FRotator YawRotation(0.0f, CameraRotation.Yaw, 0.0f); // Pitch 제거
	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveDirection = Forward * InputValue.X + Right * InputValue.Y;
	MoveDirection.Normalize();
	
	// UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), MoveDirection.X, MoveDirection.Y, MoveDirection.Z);

	float CurrentMoveSpeed = bIsGround ? MoveSpeed : MoveSpeed * AirControl;
	// UE_LOG(LogTemp, Warning, TEXT("%f * %f = %f"), MoveSpeed, AirControl, CurrentMoveSpeed);

	Velocity.X = MoveDirection.X * CurrentMoveSpeed;
	Velocity.Y = MoveDirection.Y * CurrentMoveSpeed;
	// AddActorWorldOffset(Velocity * GetWorld()->GetDeltaSeconds(), true);
}

void APlayerPawn::MoveStop(const FInputActionValue& Value)
{
	Velocity.X = 0.0f;
	Velocity.Y = 0.0f;
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
	InputValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("Input Look"));
	if (InputValue.IsNearlyZero()) return;

	float PitchInput = InputValue.Y * LookSpeed * GetWorld()->GetDeltaSeconds();
	float YawInput = InputValue.X * LookSpeed * GetWorld()->GetDeltaSeconds();

	FRotator ControlRotation = SpringArm->GetRelativeRotation(); // 입력이 될 때마다 현재 SpringArm의 Rotation값을 가져옴
	ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch - PitchInput, -80.0f, 80.0f);
	
	if (bMoveRelativeToCamera)
	{
		// SpringArm Rotation
		SpringArm->SetRelativeRotation(FRotator(ControlRotation.Pitch, ControlRotation.Yaw + YawInput, 0.0f));
	}
	else
	{
		// Actor Rotation
		AddActorWorldRotation(FRotator(0.0f, YawInput, 0.0f));
		SpringArm->SetRelativeRotation(FRotator(ControlRotation.Pitch, 0.0f, 0.0f));
	}
}

void APlayerPawn::Jump(const FInputActionValue& Value)
{
	if (bIsGround)
	{
		Velocity.Z = JumpForce;
		bIsGround = false;
	}
}

void APlayerPawn::SprintStart(const FInputActionValue& Value)
{
	MoveSpeed = SprintSpeed;
}

void APlayerPawn::SprintStop(const FInputActionValue& Value)
{
	MoveSpeed = NormalSpeed;
}
