#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerInputData.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerInputData->InputMappingContext)
			{
				Subsystem->AddMappingContext(PlayerInputData->InputMappingContext, 0);
			}
		}
	}
}
