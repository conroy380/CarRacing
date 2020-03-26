// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	class ACarRacingGameState* GameState;

protected:

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle Timer;
	
public:

	ALobbyGameMode();
	
protected:
	
	virtual void BeginPlay() override;

	void StartMatch();

public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE void SetTimer(FTimerHandle TimerHandle) { Timer = TimerHandle; }
	FORCEINLINE FTimerHandle GetTimer() { return Timer; }
};
