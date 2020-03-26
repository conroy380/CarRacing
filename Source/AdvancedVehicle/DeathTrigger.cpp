// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathTrigger.h"
#include "VehicleClass.h"
#include "GameFramework/DefaultPawn.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CarPlayerState.h"
#include "Engine/Engine.h"
#include "CarRacingGameState.h"
#include "GameFramework/PlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "CarsGameInstance.h"

ADeathTrigger::ADeathTrigger()
{
	GetCollisionComponent()->SetCollisionProfileName(FName("VehicleCollision"));

	SetReplicates(true);
}

void ADeathTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	AVehicleClass* Car;
	FTransform Transform;
	APlayerController* PlayerController;
	ACarPlayerState* PlayerState;
	
	Transform.SetLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 2500.f));
	
	Car = Cast<AVehicleClass>(OtherActor);
	
	if(Car)
	{
		//ADefaultPawn* DefaultPawn = GetWorld()->SpawnActor<ADefaultPawn>(ADefaultPawn::StaticClass(), Transform);

		PlayerController = Cast<APlayerController>(Car->GetController());

		if(PlayerController)
		{			
			PlayerController->ChangeState(NAME_Spectating);
			
			if(Role == ROLE_Authority && PlayerController->Role)
			{
				PlayerController->PlayerState->bIsSpectator = true;
			}

			//PlayerController->ClientSetLocation(Transform.GetLocation(), Transform.Rotator());

			PlayerState = PlayerController->GetPlayerState<ACarPlayerState>();

			if (HasAuthority())
			{
				if (PlayerState)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Existe"));
					
					Cast<ACarRacingGameState>(GetWorld()->GetGameState())->SetDeadPlayers(1);
					GetWorld()->GetGameState<ACarRacingGameState>()->FindPlayers();					
					PlayerState->SetIsDead(true);
				}
			}

		}

		Car->Disable();
	}
}
