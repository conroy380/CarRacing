// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class ADVANCEDVEHICLE_API AActorPool : public AActor
{
	GENERATED_BODY()

private:
	int Ind;

public:	
	// Sets default values for this actor's properties
	AActorPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<AActor*> ActorPool;

	UPROPERTY(EditAnywhere, Category = "Settings")
		int Size;

	UPROPERTY(EditAnywhere, Category = "Settings")
		class UClass* ObjectToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetActorFromPool();

	void ReturnActorToPool(AActor* PooledActor);

	void SetPoolSize(int PoolSize);

	int GetPoolSize();

	UFUNCTION(BlueprintCallable)
		void SetObjectToSpawn(class UClass* Object);

	void FillPool();
};
