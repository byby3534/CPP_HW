// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallHealPotionItem.h"
#include "PlayerPawn.h"

ASmallHealPotionItem::ASmallHealPotionItem()
{
	HealAmount = 10;
}

void ASmallHealPotionItem::ActivateItem(AActor* Activator)
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
