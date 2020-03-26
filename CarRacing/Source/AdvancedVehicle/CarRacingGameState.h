// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CarRacingGameState.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ACarRacingGameState : public AGameStateBase
{
	GENERATED_BODY()

private:

	class ALobbyGameMode* LobbyGameMode;
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated)
		float RemainingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		int MaxPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		int Players;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		int DeadPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		TArray<class AVehicleClass*> Vehicles;

public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
		void SetRemainingTime();
	
	UFUNCTION(BlueprintCallable)
		float GetRemainingTime();

	UFUNCTION(BlueprintCallable)
		void FindPlayers();
	
	FORCEINLINE void SetMaxPlayers(int Value) { MaxPlayers += Value; }
	FORCEINLINE int GetMaxPlayers() { return MaxPlayers; }
	FORCEINLINE void SetPlayers(int Value) { Players += Value; }
	FORCEINLINE int GetPlayers() { return Players; }
	FORCEINLINE void SetDeadPlayers(int Value) { DeadPlayers += Value; }
	FORCEINLINE int GetDeadPlayers() { return DeadPlayers; }
};
