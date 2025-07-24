#include "MainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonPlayerController.h"
#include "SpawnVolume.h"
#include "MainGameInstance.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AMainGameState::AMainGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	WaveDuration = 35.0f;
	CurrentWave = 0;
	CurrentLevelIndex = 0;
	MaxWaves = 3;
	MaxLevels = 3;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains("MenuLevel"))
	{
		StartLevel();
	}

	// 타이머
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMainGameState::UpdateHUD,
		0.1f,
		true
	);
}

void AMainGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AThirdPersonPlayerController* ThirdPersonPlayerController = Cast<AThirdPersonPlayerController>(PlayerController))
		{
			ThirdPersonPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			CurrentLevelIndex = MainGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	StartWave();
}

int32 AMainGameState::GetScore() const
{
	return Score;
}

void AMainGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			MainGameInstance->AddToScore(Amount);
		}
	}
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void AMainGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (0 < SpawnedCoinCount && CollectedCoinCount >= SpawnedCoinCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALL COINS COLLECTED!"));
		EndWave(true);
	}
}

void AMainGameState::SpawnItem()
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	int32 ItemToSpawn = 30 + (CurrentWave - 1) * 10;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor)
				{
					SpawnedItem.Add(SpawnedActor); // 아이템 벡터에 저장(웨이브마다 제거 하기위해 저장함)

					if (SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
				}
			}
		}
	}
}

void AMainGameState::StartWave()
{
	if (CurrentWave >= MaxWaves)
	{
		EndLevel();
		return;
	}

	for (AActor* Item : SpawnedItem)
	{
		if (Item && !Item->IsPendingKillPending())
		{
			Item->Destroy();
		}
	}
	SpawnedItem.Empty();


	CurrentWave++;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	SpawnItem();
	UE_LOG(LogTemp, Warning, TEXT("Level %d - Wave %d Start!, Spawned %d coin"), CurrentLevelIndex + 1, CurrentWave, SpawnedCoinCount);
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AMainGameState::OnWaveTimeOut, WaveDuration, false);
}

void AMainGameState::OnWaveTimeOut()
{
	EndWave(false);
}

void AMainGameState::EndWave(bool bSuccess)
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (bSuccess)
	{
		// 다음 웨이브 진행
		StartWave();
	}
	else
	{
		// 실패 시 게임 재시작
		OnGameOver(true);
	}

}


void AMainGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			MainGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	// 최대 레벨인경우 게임종료 로직
	if (CurrentLevelIndex >= MaxLevels)
	{
		// 게임 클리어 출력
		OnGameOver(true);
		return;
	}

	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		// 맵 이름이 없으면 게임오버
		//OnGameOver(true);
	}
}

void AMainGameState::OnGameOver(bool bIsRestart)
{

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AThirdPersonPlayerController* ThirdPersonPlayerController = Cast<AThirdPersonPlayerController>(PlayerController))
		{
			ThirdPersonPlayerController->SetPause(true);
			ThirdPersonPlayerController->ShowMainMenu(bIsRestart);
		}
	}
}

void AMainGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AThirdPersonPlayerController* ThirdPersonPlayerController = Cast<AThirdPersonPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = ThirdPersonPlayerController->GetHudWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
						if (MainGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MainGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d - %d"), CurrentLevelIndex + 1, CurrentWave)));
				}

				if (UTextBlock* CoinCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CoinCount"))))
				{
					CoinCountText->SetText(FText::FromString(FString::Printf(TEXT("Coin: %d / %d"), CollectedCoinCount, SpawnedCoinCount)));
				}
			}
		}
	}
}
