// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "BuildingMesh.h"
#include "RoadSection.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "VehicleClass.h"
#include "ActorPool.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingMesh = CreateDefaultSubobject<UBuildingMesh>(TEXT("Building Mesh"));
	RootComponent = BuildingMesh;
	BuildingMesh->SetIsReplicated(true);
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RoofBuildingMesh = CreateDefaultSubobject<UBuildingMesh>(TEXT("Roof Mesh"));
	RoofBuildingMesh->SetupAttachment(BuildingMesh, FName("Roof"));
	RoofBuildingMesh->SetIsReplicated(true);

	WallBuildingMesh = CreateDefaultSubobject<UBuildingMesh>(TEXT("Wall Mesh"));
	WallBuildingMesh->SetupAttachment(BuildingMesh, FName("Wall"));
	WallBuildingMesh->SetIsReplicated(true);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BuildingMesh->SetRandomMesh();

		RoofBuildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
		WallBuildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));

		RoofBuildingMesh->SetRandomMesh();
		WallBuildingMesh->SetRandomMesh();
	}

	Disable();
	//BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ShouldEnable)
	{
		Enable();
		ShouldEnable = false;
	}
}

void ABuilding::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Overlaping"));
	
	AVehicleClass* Vehicle;

	Vehicle = Cast<AVehicleClass>(OtherActor);
	
	if(!Vehicle)
	{
		Disable();
	}
}

void ABuilding::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	ARoadSection* RoadSection;

	RoadSection = Cast<ARoadSection>(OtherActor);
	
	if (RoadSection)
	{
		GetWorldTimerManager().SetTimer(EnableTimer, this, &ABuilding::Enable, 3.0f, false);
	}
}

void ABuilding::ReturnActorToPool()
{
	if (OwnerPool)
	{
		OwnerPool->ReturnActorToPool(this);
		Disable();
	}
}

void ABuilding::Disable()
{
	//if (HasAuthority())
	//{
		BuildingMesh->SetHiddenInGame(true);
		RoofBuildingMesh->SetHiddenInGame(true);
		WallBuildingMesh->SetHiddenInGame(true);
	//}
	
	//SetActorEnableCollision(false);
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABuilding::Enable()
{
	//SetActorEnableCollision(true);
	
	//if (HasAuthority())
	//{
		BuildingMesh->SetHiddenInGame(false);
		RoofBuildingMesh->SetHiddenInGame(false);
		WallBuildingMesh->SetHiddenInGame(false);
	//	BuildingMesh->SetRandomMesh();

	//	RoofBuildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
	//	WallBuildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));

	//	RoofBuildingMesh->SetRandomMesh();
	//	WallBuildingMesh->SetRandomMesh();
	//}
	
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABuilding::SetOwnerPool(AActorPool* Pool)
{
	OwnerPool = Pool;
}

