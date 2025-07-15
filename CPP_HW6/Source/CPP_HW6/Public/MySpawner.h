#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpawner.generated.h"

UCLASS()
class CPP_HW6_API AMySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMySpawner();

protected:
	virtual void BeginPlay() override;

	void SpawnMyActor(TSubclassOf<AActor> PlatformClass);
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> PlatformClass_A;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int NumPlatforms;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float LocationZ;
};
