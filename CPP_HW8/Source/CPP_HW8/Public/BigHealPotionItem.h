// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealPotionItem.h"
#include "BigHealPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_HW8_API ABigHealPotionItem : public AHealPotionItem
{
	GENERATED_BODY()
public:
	ABigHealPotionItem();

	virtual void ActivateItem(AActor* Activator) override;

};
