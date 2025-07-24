// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "PlayerPawn.h"
#include "ThirdPersonPlayerController.h"

AMainGameMode::AMainGameMode()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = AThirdPersonPlayerController::StaticClass();
}
