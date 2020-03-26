// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"
#include "Engine/Engine.h"

// Sets default values
AActorPool::AActorPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ind = -1;
}

// Called when the game starts or when spawned
void AActorPool::BeginPlay()
{
	Super::BeginPlay();

	if (!(Ind >= 0))
	{
		Ind = -1;
	}

	ActorPool.Reserve(Size);
}

// Called every frame
void AActorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AActorPool::GetActorFromPool()
{
	if (Ind >= 0)
	{
		AActor* ActorFromPool = ActorPool[Ind];

		ActorPool.RemoveAt(Ind);
		--Ind;

		return ActorFromPool;
	}

	return nullptr;
}

void AActorPool::ReturnActorToPool(AActor* PooledActor)
{
	ActorPool.Add(PooledActor);
	PooledActor->SetActorLocation(GetActorLocation());
	++Ind;
}

void AActorPool::SetPoolSize(int PoolSize)
{
	Size = PoolSize;
}

int AActorPool::GetPoolSize()
{
	return  Ind;
}

void AActorPool::SetObjectToSpawn(UClass* Object)
{
	if (Object)
	{
		ObjectToSpawn = Object;
	}
}

void AActorPool::FillPool()
{
	UWorld* world = GetWorld();

	if (ObjectToSpawn)
	{

		if (world)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Han spawneado nuevos enemigos"));
			for (int i = 0; i < Size; ++i)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;

				AActor* PooledActor = world->SpawnActor<AActor>(ObjectToSpawn, GetActorLocation(), FRotator::ZeroRotator, spawnParams);

				if (PooledActor)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, Enemy->GetName());

					ActorPool.Add(PooledActor);

					++Ind;
				}
			}
			++Ind;
			--Ind;
		}
	}
}
