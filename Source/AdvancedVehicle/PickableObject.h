// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableObject.generated.h"

/**
 * 
 */
UCLASS(abstract)
class ADVANCEDVEHICLE_API APickableObject : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Object Settings")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Object Settings")
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Object Settings")
		class URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Object Settings")
		class UBoxComponent* BoxComponent;
	
public:
	APickableObject();

};
