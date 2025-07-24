// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PotionItem.generated.h"


UCLASS()
class CPP_HW8_API APotionItem : public ABaseItem
{
	GENERATED_BODY()
protected:
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText PotionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText PotionDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FName PotionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UTexture2D* PotionIcon;
};
