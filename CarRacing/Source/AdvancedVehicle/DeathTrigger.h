// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DeathTrigger.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ADeathTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ADeathTrigger();
protected:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
