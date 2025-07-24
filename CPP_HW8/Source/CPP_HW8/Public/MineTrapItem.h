// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapItem.h"
#include "MineTrapItem.generated.h"

UCLASS()
class CPP_HW8_API AMineTrapItem : public ATrapItem
{
	GENERATED_BODY()
	
public:
	AMineTrapItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* DamageCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;

	bool bHasExploded;
	FTimerHandle ExplosionTimerHandle;
	void ActivateItem(AActor* Activator) override;
	void Explode();
};
