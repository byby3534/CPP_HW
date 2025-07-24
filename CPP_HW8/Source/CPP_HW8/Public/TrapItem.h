// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "TrapItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_HW8_API ATrapItem : public ABaseItem
{
	GENERATED_BODY()
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ActivationDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float TriggerRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DamageRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName TrapType;
};
