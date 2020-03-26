// Fill out your copyright notice in the Description page of Project Settings.


#include "TurboObject.h"
#include "VehicleClass.h"
#include "WheeledVehicleMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

ATurboObject::ATurboObject()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TurboMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));

	StaticMeshComponent->SetStaticMesh(TurboMesh.Object);
	
	CollisionComponent->SetSphereRadius(95);

	BoxOverlapDelgate.BindUFunction(this, FName("OnBoxOverlap"));
	BoxComponent->OnComponentBeginOverlap.Add(BoxOverlapDelgate);

	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -60.f));
}

void ATurboObject::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetSimulatePhysics(true);
}


void ATurboObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	AVehicleClass* Vehicle;
	
	Vehicle = Cast<AVehicleClass>(OtherActor);
	
	if(Vehicle)
	{
		Vehicle->ActivateTurbo();
		
		Destroy();
	}

	
}

void ATurboObject::OnBoxOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != this)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Red, OtherActor->GetName());
		CollisionComponent->SetSimulatePhysics(false);
		//Destroy();
	}
}
