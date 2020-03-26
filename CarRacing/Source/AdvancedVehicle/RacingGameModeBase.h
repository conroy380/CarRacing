// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoadEnumerator.h"
#include "Runnable.h"
#include "RacingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API ARacingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	FTransform NextSpawnPoint;
	int UpRoad;
	int DownRoad;
	int RightRoad;
	int LeftRoad;
	int StartPointIndex = 0;	
	class ARoadSection* NewRoadSection;
	FTimerHandle SpawnPickableHandle;
	TSubclassOf<class AVehicleClass> Vehicle;
	TSubclassOf<class ABuilding> BuildingClass;
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Settings")
		TArray<TSubclassOf<class ARoadSection>> Road;

	UPROPERTY(BlueprintReadWrite)
		TArray<class APlayerStart*> StartPoint;

	UPROPERTY(EditAnywhere)
		class AActorPool* BuildingPool;
	
	ERoadEnumerator RoadEnumerator;

protected:
	virtual void BeginPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
public:
	ARacingGameModeBase();

	UFUNCTION(BlueprintCallable)
		void AddRoad();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
		void GetStartPoints();

	UFUNCTION(BlueprintCallable)
		void SetNextSpawnPoint(FTransform Transform);

	UFUNCTION(BlueprintCallable)
		ABuilding* GetBuilding();

	UFUNCTION(BlueprintCallable)
		void CreateCity(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize);

	UFUNCTION(BlueprintCallable)
		void CreateCityAsync(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize);
	
	FORCEINLINE FTimerHandle GetSpawnPickableHandle() { return SpawnPickableHandle; }
	FORCEINLINE void SetSpawnPickableHandle(FTimerHandle TimerHandle) { SpawnPickableHandle = TimerHandle; }
};

class CityGenerationAsyncTask : public FNonAbandonableTask
{
	float XModifier;
	float YModifier;
	float ZModifier;
	int GridSize;
	float TileSize;
	ARacingGameModeBase* GameMode;

public:
	/*Default constructor*/
	CityGenerationAsyncTask(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize, ARacingGameModeBase* GameMode);
	
	~CityGenerationAsyncTask();

	/*This function is needed from the API of the engine.
	My guess is that it provides necessary information
	about the thread that we occupy and the progress of our task*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CityGenerationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	/*This function is executed when we tell our task to execute*/
	void DoWork();
};