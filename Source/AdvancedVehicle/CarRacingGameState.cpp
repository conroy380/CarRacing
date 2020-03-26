// Fill out your copyright notice in the Description page of Project Settings.


#include "CarRacingGameState.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "VehicleClass.h"
#include "EngineUtils.h"
#include "CarPlayerState.h"

void ACarRacingGameState::BeginPlay()
{
	Super::BeginPlay();

	LobbyGameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
}

void ACarRacingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACarRacingGameState, MaxPlayers);
	DOREPLIFETIME(ACarRacingGameState, Players);
	DOREPLIFETIME(ACarRacingGameState, DeadPlayers);
	DOREPLIFETIME(ACarRacingGameState, RemainingTime);
	DOREPLIFETIME(ACarRacingGameState, Vehicles);
}

void ACarRacingGameState::SetRemainingTime()
{
	if (LobbyGameMode)
	{
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(LobbyGameMode->GetTimer());
		RemainingTime = FMath::Clamp(RemainingTime, 0.0f, 60.0f);
	}
}

float ACarRacingGameState::GetRemainingTime()
{
	SetRemainingTime();
	
	return FMath::RoundToFloat(RemainingTime);
}

void ACarRacingGameState::FindPlayers()
{
	UWorld* World = GetWorld();
	ACarPlayerState* PlayerState;

	Vehicles.Empty();

	if (World && HasAuthority())
	{
		for (TActorIterator<AVehicleClass> Itr(World); Itr; ++Itr)
		{
			AVehicleClass* Vehicle = *Itr;

			if (Vehicle)
			{
				PlayerState = Vehicle->GetPlayerState<ACarPlayerState>();

				if (PlayerState && !PlayerState->GetIsDead())
				{
					Vehicles.Add(Vehicle);
				}
			}
		}
	}
}