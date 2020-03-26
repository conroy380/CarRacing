// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingMesh.h"
#include "ConstructorHelpers.h"
#include "VehicleClass.h"
#include "RoadSection.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

UBuildingMesh::UBuildingMesh()
{
	//ConstructorHelpers::FObjectFinder<UStaticMesh> BuildingMesh(TEXT("/Game/Proyect/StaticMeshes/Enviroment/Meshes/SM_Building_01"));

	//Meshes.Add(BuildingMesh.Object);

	BeginOverlapDelegate.BindUFunction(this, FName("BeginOverlap"));
	OnComponentBeginOverlap.Add(BeginOverlapDelegate);
	
	EndOverlapDelegate.BindUFunction(this, FName("EndOverlap"));
	OnComponentEndOverlap.Add(EndOverlapDelegate);
	
	SetCollisionProfileName(FName("NoCollision"));

	SetCastShadow(false);
	
	LDMaxDrawDistance = 30000;
	MinDrawDistance = 20;
}

void UBuildingMesh::BeginPlay()
{
	Super::BeginPlay();

	SetRandomMesh();
	
	UpdateOverlaps();
	
}

void UBuildingMesh::SetRandomMesh()
{
	int Ind;

	Ind = FMath::RandRange(0, Meshes.Num() - 1);

	if (Meshes.Num() > 0)
	{
		SetStaticMesh(Meshes[Ind]);
	}
}

void UBuildingMesh::SetOwner(AActor* Owner)
{
	OwnerParent = Owner;
}

void UBuildingMesh::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AVehicleClass* Vehicle;
	UBuildingMesh* BuildingMesh;
	
	Vehicle = Cast<AVehicleClass>(OtherActor);
	BuildingMesh = Cast<UBuildingMesh>(OtherComp);
	
	if(OtherActor != OwnerParent && OtherComp->GetAttachmentRootActor() != OwnerParent)
	{
		ObjectOverlapped.Add(OtherActor);
		Disable();
	}
}

void UBuildingMesh::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//ARoadSection* RoadSection;
	//UBuildingMesh* BuildingMesh;
	//
	//RoadSection = Cast<ARoadSection>(OtherComp);
	//BuildingMesh = Cast<UBuildingMesh>(OtherActor);

	//AVehicleClass* Vehicle;
	//Vehicle = Cast<AVehicleClass>(OtherActor);
	//
	//if (!Vehicle)
	//{
	//	GetWorld()->GetTimerManager().SetTimer(EnableTimer, this, &UBuildingMesh::Enable, 3.0f, false);
	//}
}

void UBuildingMesh::Enable()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetHiddenInGame(false);
}


void UBuildingMesh::Disable()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetHiddenInGame(true);
}
