// Fill out your copyright notice in the Description page of Project Settings.


#include "MineTrapItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineTrapItem::AMineTrapItem()
{
	TriggerRadius = 50.0f;
	TriggerCollision->InitSphereRadius(TriggerRadius);

	DamageRadius = 200.0f;
	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DamageCollision"));
	DamageCollision->InitSphereRadius(DamageRadius);
	DamageCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DamageCollision->SetupAttachment(Scene);

	ActivationDelay = 0.5f;
	DamageAmount = 10.0f;
	TrapType = "Mine";
	bHasExploded = false;
}

void AMineTrapItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;

	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineTrapItem::Explode, ActivationDelay, false);

	bHasExploded = true;
}

void AMineTrapItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	DamageCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				DamageAmount,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	DestroyItem();
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);
	}
}
