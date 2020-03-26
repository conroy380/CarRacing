// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "CarsGameInstance.h"
#include "CarRacingGameState.h"

ALobbyGameMode::ALobbyGameMode()
{
	
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameState->SetMaxPlayers(GetGameInstance<UCarsGameInstance>()->GetMaxPlayers());
	
	GetWorldTimerManager().SetTimer(Timer, this, &ALobbyGameMode::StartMatch, 60.0f);

	//GameState->SetRemainingTime(60.0f);
}

void ALobbyGameMode::StartMatch()
{
	bUseSeamlessTravel = true;
	GetGameInstance<UCarsGameInstance>()->ChangeLevel("Game1");
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GameState = GetGameState<ACarRacingGameState>();
	
	if (GameState)
	{
		GameState->SetPlayers(1);

		if (GameState->GetPlayers() == GameState->GetMaxPlayers())
		{
			StartMatch();
		}
	}
}

void ALobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
