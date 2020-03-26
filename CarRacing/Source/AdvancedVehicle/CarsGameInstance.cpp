// Fill out your copyright notice in the Description page of Project Settings.


#include "CarsGameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "CarPlayerState.h"
#include "VehicleClass.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

//void UCarsGameInstance::HostSession()
//{
//	
//}
//
//void UCarsGameInstance::JoinSession()
//{
//	
//}

void UCarsGameInstance::Init()
{
	Super::Init();

	Subsystem = IOnlineSubsystem::Get();

	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();

		if(SessionInterface)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCarsGameInstance::OnCreateSessionComplete);
		}
	}
}

void UCarsGameInstance::ChangeLevel(FString LevelName)
{
	GetWorld()->ServerTravel(LevelName);
}

void UCarsGameInstance::HostSession(FString SessionName, int Players, bool IsLan)
{
	UWorld* World = GetWorld();

	const FName Session = *SessionName;
	FOnlineSessionSettings SessionSettings;
	
	SessionSettings.NumPublicConnections = Players;
	SessionSettings.bIsLANMatch = IsLan;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	if(SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(Session);

		if(ExistingSession)
		{
			SessionInterface->DestroySession(Session);
		}

		SessionInterface->CreateSession(0, Session, SessionSettings);

		MaxPlayers = Players;
	}
}

void UCarsGameInstance::OnCreateSessionComplete(FName SessionName, bool Successful)
{
	UWorld* World = GetWorld();
	
	if (World && Successful)
	{
		World->ServerTravel("/Game/Proyect/Levels/Lobby?listen", true);
	}
}

void UCarsGameInstance::Join()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if(PlayerController)
	{
		PlayerController->ClientTravel(FString(""), ETravelType::TRAVEL_Absolute);
	}
}
