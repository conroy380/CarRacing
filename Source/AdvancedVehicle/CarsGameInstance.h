// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "CarsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API UCarsGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	class IOnlineSubsystem* Subsystem;
	IOnlineSessionPtr SessionInterface;
	
protected:

	UPROPERTY(BlueprintReadWrite)
		int MaxPlayers;
	
public:
	UFUNCTION(BlueprintCallable)
		void HostSession(FString SessionName, int Players, bool IsLan);

	void OnCreateSessionComplete(FName SessionName, bool Successful);
	
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	//	void JoinSession();

	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
		void ChangeLevel(FString LevelName);

	//UFUNCTION(BlueprintCallable)
	//	void Host();

	UFUNCTION(BlueprintCallable)
		void Join();
	
	FORCEINLINE void SetMaxPlayers(int Players) { MaxPlayers = Players; }
	FORCEINLINE int GetMaxPlayers() { return MaxPlayers; }
};
