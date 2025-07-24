// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PotionItem.h"
#include "HealPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_HW8_API AHealPotionItem : public APotionItem
{
	GENERATED_BODY()
	
public:
	AHealPotionItem();
protected:
	int32 HealAmount;
};
