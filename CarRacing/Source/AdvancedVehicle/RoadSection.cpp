// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadSection.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "BuildingMesh.h"
#include "RacingGameModeBase.h"
#include "TurboObject.h"
#include "TimerManager.h"
#include "DeathTrigger.h"
#include "Engine/Engine.h"

// Sets default values
ARoadSection::ARoadSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Defaul Root"));
	DefaultRootComponent->SetupAttachment(RootComponent);
	
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	InstancedStaticMeshComponent->SetupAttachment(DefaultRootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(DefaultRootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(DefaultRootComponent);

	PickableObjectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickable Object Area"));
	PickableObjectArea->SetupAttachment(DefaultRootComponent);
	PickableObjectArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Building = CreateDefaultSubobject<UBuildingMesh>(TEXT("Building"));
	//Building->SetupAttachment(DefaultRootComponent);

	//Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	//Floor->SetupAttachment(DefaultRootComponent);
	//
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ARoadSection::BeginPlay()
{
	Super::BeginPlay();

	CarGameMode = GetWorld()->GetAuthGameMode<ARacingGameModeBase>();
	
	GetComponents<UBuildingMesh>(BuildingMeshes);
	SetBuildingsZLocation();

	if (CarGameMode)
	{
		if (!GetWorldTimerManager().IsTimerActive(CarGameMode->GetSpawnPickableHandle()))
		{
			SpawnPickableObject();
		}
	}

	FVector Location;
	
	ADeathTrigger* DeathTrigger = GetWorld()->SpawnActor<ADeathTrigger>(ADeathTrigger::StaticClass(), GetActorTransform());

	Location = GetActorLocation();
	Location.Z = -1900.0f;

	DeathTrigger->SetActorLocation(Location);
	DeathTrigger->SetActorScale3D(FVector(75.0f, 75.0f, 5.0f));
}

// Called every frame
void ARoadSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoadSection::SpawnPickableObject()
{
	FVector SpawnPoint;
	FTimerHandle TimerHandle;
	FActorSpawnParameters SpawnParams;
	FVector BoxExtent;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	
	BoxExtent = PickableObjectArea->GetUnscaledBoxExtent();

	if(GetActorRotation().Yaw > 0)
	{
		BoxExtent.X = BoxExtent.Y;
		BoxExtent.Y = PickableObjectArea->GetUnscaledBoxExtent().X;
	}

	
	FVector Min = PickableObjectArea->GetComponentLocation() - PickableObjectArea->GetUnscaledBoxExtent();
	FVector Max = PickableObjectArea->GetComponentLocation() + PickableObjectArea->GetUnscaledBoxExtent();

	SpawnPoint = FMath::RandPointInBox(FBox(Min, Max));
	
	GetWorld()->SpawnActor<ATurboObject>(SpawnPoint, FRotator::ZeroRotator, SpawnParams);

	GetWorldTimerManager().SetTimer(TimerHandle, FMath::RandRange(1, 10), false);

	if (CarGameMode)
	{
		CarGameMode->SetSpawnPickableHandle(TimerHandle);
	}
}

void ARoadSection::SetBuildingsZLocation()
{
	//Floor->SetWorldLocation(FVector(Floor->GetComponentLocation().X, Floor->GetComponentLocation().Y, -1900));
	
	for(int i = 0; i < BuildingMeshes.Num(); ++i)
	{
		BuildingMeshes[i]->SetOwner(this);
		BuildingMeshes[i]->SetCollisionProfileName(FName("Building"));
		BuildingMeshes[i]->SetWorldLocation(FVector(BuildingMeshes[i]->GetComponentLocation().X, BuildingMeshes[i]->GetComponentLocation().Y, -1900.0f));
	}
}



