// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "RoadSection.generated.h"


UCLASS()
class ADVANCEDVEHICLE_API ARoadSection : public AActor
{
	GENERATED_BODY()

private:
	USceneComponent* DefaultRootComponent;

	class ARacingGameModeBase* CarGameMode;


	FScriptDelegate BuildingMeshBeginOverlap;
	
public:	
	// Sets default values for this actor's properties
	ARoadSection();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UBoxComponent* PickableObjectArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Road Settings")
		TArray<class UBuildingMesh*> BuildingMeshes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UBuildingMesh* Building;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Road Settings")
		class UStaticMeshComponent* Floor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SpawnPickableObject();

	UFUNCTION(BlueprintCallable)
		void SetBuildingsZLocation();
	
	FORCEINLINE FTransform GetAttachPoint() { return ArrowComponent->GetComponentTransform(); }
};
