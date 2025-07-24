// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_HW8_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AThirdPersonPlayerController();
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "MENU")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "MENU")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHudWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
protected:
	virtual void BeginPlay() override;
};
