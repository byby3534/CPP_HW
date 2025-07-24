// Fill out your copyright notice in the Description page of Project Settings.


#include "BigHealPotionItem.h"
#include "PlayerPawn.h"

ABigHealPotionItem::ABigHealPotionItem()
{
	HealAmount = 20;
}

void ABigHealPotionItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		APlayerPawn* PlayerCharacter = Cast<APlayerPawn>(Activator);
		if (PlayerCharacter)
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
