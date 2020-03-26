// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPlayerState.h"
#include "Net/UnrealNetwork.h"

ACarPlayerState::ACarPlayerState()
{
	
}

ACarPlayerState::ACarPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IsDead = false;
}

void ACarPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACarPlayerState, IsDead);
}

bool ACarPlayerState::GetIsDead()
{
	return IsDead;
}
