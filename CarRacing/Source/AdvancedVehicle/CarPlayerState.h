// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CarPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ACarPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()

public:
	ACarPlayerState();
	
protected:
	
	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsDead;

public:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	FORCEINLINE void SetIsDead(bool b) { IsDead = b; };

	UFUNCTION(BlueprintCallable)
		bool GetIsDead();
};
