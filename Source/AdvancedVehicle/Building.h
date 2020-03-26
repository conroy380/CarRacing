// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class ADVANCEDVEHICLE_API ABuilding : public AActor
{
	GENERATED_BODY()

private:

	FTimerHandle EnableTimer;
	class AActorPool* OwnerPool;
	bool ShouldEnable;
	
protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UBuildingMesh* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UBuildingMesh* RoofBuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UBuildingMesh* WallBuildingMesh;
	
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void ReturnActorToPool();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Disable();

	UFUNCTION(BlueprintCallable)
		void Enable();

	UFUNCTION(BlueprintCallable)
		void SetOwnerPool(class AActorPool* Pool);

	FORCEINLINE void SetShouldEnable(bool bShouldEnable) { ShouldEnable = bShouldEnable; }
};
