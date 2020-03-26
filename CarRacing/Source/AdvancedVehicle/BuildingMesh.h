// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingMesh.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ADVANCEDVEHICLE_API UBuildingMesh : public UStaticMeshComponent
{
	GENERATED_BODY()

private:

	FTimerHandle EnableTimer;
	AActor* OwnerParent;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		TArray<class UStaticMesh*> Meshes;


	UPROPERTY(VisibleAnywhere, Category = "Actor")
		TArray<UObject*> ObjectOverlapped;
	
	FScriptDelegate BeginOverlapDelegate;
	FScriptDelegate EndOverlapDelegate;
	
public:

	UBuildingMesh();

protected:

	virtual void BeginPlay() override;

public:
	void SetRandomMesh();

	void SetOwner(AActor* Owner);
	
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
		void Enable();

	UFUNCTION(BlueprintCallable)
		void Disable();

};