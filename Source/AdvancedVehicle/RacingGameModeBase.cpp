// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingGameModeBase.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "RoadSection.h"
#include "TimerManager.h"
#include "CarRacingGameState.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "VehicleClass.h"
#include "Building.h"
#include "ActorPool.h"
#include "ParallelFor.h"

namespace Threading
{
	ARacingGameModeBase* GameMode;

	void CreateCityThread(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize)
	{
		FTransform BuildingTransform;
		float XPos = 0;
		float YPos = 0;

		for (int i = 0; i <= GridSize; ++i)
		{
			XPos = ((i * TileSize) - ((GridSize * TileSize) / 2)) + XModifier;

			for (int j = 0; j <= GridSize; ++j)
			{
				YPos = ((j * TileSize) - ((GridSize * TileSize) / 2)) + YModifier;

				BuildingTransform.SetLocation(FVector(XPos, YPos, ZModifier));

				ABuilding* Building = GameMode->GetBuilding();

				if (Building)
				{
					Building->SetActorTransform(BuildingTransform);
					Building->Enable();
				}
			};
		};
	}
}

ARacingGameModeBase::ARacingGameModeBase()
{
	ConstructorHelpers::FClassFinder<ARoadSection> Straight(TEXT("/Game/Proyect/Blueprints/BP_Carretera_Tile01Recta"));
	ConstructorHelpers::FClassFinder<ARoadSection> Up(TEXT("/Game/Proyect/Blueprints/BP_Carretera_Tile02Subida"));
	ConstructorHelpers::FClassFinder<ARoadSection> Down(TEXT("/Game/Proyect/Blueprints/BP_Carretera_Tile03Bajada"));
	ConstructorHelpers::FClassFinder<ARoadSection> Right(TEXT("/Game/Proyect/Blueprints/BP_Carretera_Tile05_Derecha"));
	ConstructorHelpers::FClassFinder<ARoadSection> Left(TEXT("/Game/Proyect/Blueprints/BP_Carretera_Tile04_Izquierda"));
	ConstructorHelpers::FClassFinder<AVehicleClass> Car(TEXT("/Game/Proyect/Blueprints/BP_Vehiculo1"));
	ConstructorHelpers::FClassFinder<ABuilding> Building(TEXT("/Game/Proyect/Blueprints/BP_Building"));
	
	Road.Add(Straight.Class);
	Road.Add(Up.Class);
	Road.Add(Down.Class);
	Road.Add(Right.Class);
	Road.Add(Left.Class);

	Vehicle = Car.Class;
	BuildingClass = Building.Class;

	NextSpawnPoint.SetLocation(FVector(-82300.0f, 0.0f, 0.0f));
}

void ARacingGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	FActorSpawnParameters spawnParams;

	spawnParams.Owner = this;

	//if (World)
	//{
	//	BuildingPool = World->SpawnActor<AActorPool>(AActorPool::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	//	if (BuildingPool)
	//	{
	//		BuildingPool->SetPoolSize(3000);
	//		BuildingPool->SetObjectToSpawn(BuildingClass);
	//		BuildingPool->FillPool();
	//	}
	//}
	
	GetWorldTimerManager().SetTimer(SpawnPickableHandle, FMath::RandRange(0.0f, 1.0f), false);

	for(int i = 0; i <= 20; ++i)
	{
		AddRoad();
	}

	//CreateCity(-40410.0f, 0.0f, -1900.0F, 20, 6000.0f);
}

void ARacingGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	GetStartPoints();
}

void ARacingGameModeBase::AddRoad()
{
	RoadEnumerator = static_cast<ERoadEnumerator>(FMath::RandRange(0, Road.Num() - 1));

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;
	
	switch(RoadEnumerator)
	{
		case ERoadEnumerator::Straight:

		NewRoadSection = GetWorld()->SpawnActor<ARoadSection>(Road[0], NextSpawnPoint, spawnParams);

		if (NewRoadSection)
		{
			NextSpawnPoint = NewRoadSection->GetAttachPoint();
		}
		
		break;

		case ERoadEnumerator::Up:

		if(UpRoad >= 3)
		{
			AddRoad();
			return;
		}

		NewRoadSection = GetWorld()->SpawnActor<ARoadSection>(Road[1], NextSpawnPoint, spawnParams);

		if (NewRoadSection)
		{
			NextSpawnPoint = NewRoadSection->GetAttachPoint();
			++UpRoad;
			--DownRoad;
		}

		UpRoad = FMath::Clamp(UpRoad,0, 3);
		DownRoad = FMath::Clamp(DownRoad, 0, 3);
		
		break;

		case ERoadEnumerator::Down:
		
		if (DownRoad >= 3)
		{
			AddRoad();
			return;
		}

		NewRoadSection = GetWorld()->SpawnActor<ARoadSection>(Road[2], NextSpawnPoint, spawnParams);

		if (NewRoadSection)
		{
			NextSpawnPoint = NewRoadSection->GetAttachPoint();
			++DownRoad;
			--UpRoad;
		}
		
		UpRoad = FMath::Clamp(UpRoad, 0, 3);
		DownRoad = FMath::Clamp(DownRoad, 0, 3);
		
		break;

		case ERoadEnumerator::Right:

		if (RightRoad >= 1)
		{
			AddRoad();
			return;
		}

		NewRoadSection = GetWorld()->SpawnActor<ARoadSection>(Road[3], NextSpawnPoint, spawnParams);

		if (NewRoadSection)
		{
			NextSpawnPoint = NewRoadSection->GetAttachPoint();
			++RightRoad;
			--LeftRoad;
		}

		RightRoad = FMath::Clamp(RightRoad, 0, 1);
		LeftRoad = FMath::Clamp(LeftRoad, 0, 1);

		break;

		case ERoadEnumerator::Left:

		if (LeftRoad >= 1)
		{
			AddRoad();
			return;
		}

		NewRoadSection = GetWorld()->SpawnActor<ARoadSection>(Road[4], NextSpawnPoint, spawnParams);

		if (NewRoadSection)
		{
			NextSpawnPoint = NewRoadSection->GetAttachPoint();
			++LeftRoad;
			--RightRoad;
		}

		RightRoad = FMath::Clamp(RightRoad, 0, 1);
		LeftRoad = FMath::Clamp(LeftRoad, 0, 1);

		break;
	}

	return;
}

void ARacingGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* World = GetWorld();
	FActorSpawnParameters spawnParams;
	
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;
	
	if (World)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Existe"));
		GetGameState<ACarRacingGameState>()->SetPlayers(1);

		AVehicleClass* NewCar = World->SpawnActor<AVehicleClass>(Vehicle, StartPoint[StartPointIndex]->GetActorTransform(), spawnParams);

		NewPlayer->Possess(NewCar);
		
		
		++StartPointIndex;
	}
}

void ARacingGameModeBase::GetStartPoints()
{
	UWorld* World = GetWorld();

	if (World)
	{
		for (TActorIterator<APlayerStart> Itr(World); Itr; ++Itr)
		{
			if(*Itr)
			{
				StartPoint.Add(*Itr);
			}
		}
	}
}

void ARacingGameModeBase::SetNextSpawnPoint(FTransform Transform)
{
	NextSpawnPoint = Transform;
}

ABuilding* ARacingGameModeBase::GetBuilding()
{
	if(BuildingPool)
	{
		ABuilding* Building = Cast<ABuilding>(BuildingPool->GetActorFromPool());

		if(Building)
		{
			Building->SetOwnerPool(BuildingPool);
			//Building->Enable();
			return Building;
		}
	}
	return nullptr;
}

void ARacingGameModeBase::CreateCity(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize)
{
	Threading::GameMode = this;
	Threading::CreateCityThread(XModifier, YModifier, ZModifier, GridSize, TileSize);
}

void ARacingGameModeBase::CreateCityAsync(float XModifier, float YModifier, float ZModifier, int GridSize, float TileSize)
{
	
	(new FAutoDeleteAsyncTask<CityGenerationAsyncTask>(XModifier, YModifier, ZModifier, GridSize, TileSize, this))->StartBackgroundTask();
}

/********************************************************************************************************************************************/

CityGenerationAsyncTask::CityGenerationAsyncTask(float XModifier, float YModifier, float ZModifier, int GridSize,
	float TileSize, ARacingGameModeBase* GameMode)
{
	this->XModifier = XModifier;
	this->YModifier = YModifier;
	this->ZModifier = ZModifier;
	this->GridSize = GridSize;
	this->TileSize = TileSize;
	this->GameMode = GameMode;
}

CityGenerationAsyncTask::~CityGenerationAsyncTask()
{
}

void CityGenerationAsyncTask::DoWork()
{
	FTransform BuildingTransform;
	float XPos = 0;
	float YPos = 0;

	for (int i = 0; i <= GridSize; ++i)
	{
		XPos = ((i * TileSize) - ((GridSize * TileSize) / 2)) + XModifier;

		for (int j = 0; j <= GridSize; ++j)
		{
			YPos = ((j * TileSize) - ((GridSize * TileSize) / 2)) + YModifier;

			BuildingTransform.SetLocation(FVector(XPos, YPos, ZModifier));

			ABuilding* Building = GameMode->GetBuilding();

			if (Building)
			{
				Building->SetActorTransform(BuildingTransform);
				Building->SetShouldEnable(true);
				//Building->Enable();
			}
		}
	}

	return;
}

