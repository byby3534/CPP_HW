// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealPotionItem.h"
#include "SmallHealPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_HW8_API ASmallHealPotionItem : public AHealPotionItem
{
	GENERATED_BODY()
	
public:
	ASmallHealPotionItem();

	virtual void ActivateItem(AActor* Activator) override;
};
