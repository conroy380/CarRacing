// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableObject.h"
#include "TurboObject.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ATurboObject : public APickableObject
{
	GENERATED_BODY()

protected:

	FScriptDelegate BoxOverlapDelgate;

protected:
	
	virtual void BeginPlay() override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	ATurboObject();
};