// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "VehicleClass.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDVEHICLE_API AVehicleClass : public AWheeledVehicle
{
	GENERATED_BODY()

private:
	FRotator DefaultCameraRotation;
	bool IsTurboActivated = false;
	FTimerHandle TurboHandle;
	FVector TurboVelocity;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
		class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Vehicle Settings")
		class USkeletalMeshComponent* Wheel1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Vehicle Settings")
		class USkeletalMeshComponent* Wheel2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Vehicle Settings")
		class USkeletalMeshComponent* Wheel3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Vehicle Settings")
		class USkeletalMeshComponent* Wheel4;
	
public:
	AVehicleClass();

	virtual void Tick(float DeltaSeconds) override;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// Move Forward
	void MoveForward(float value);

	// Move Right
	void MoveRight(float value);

	// Brake
	void Brake(float value);

	//Handbrake
	void HandbrakePressed();

	void HandbrakeReleased();

	void PlayAudio();

	UFUNCTION(BlueprintCallable)
		void FreeCameraLook();
	
	UFUNCTION(BlueprintCallable)
		void SetReverseBrake(bool b);

	UFUNCTION(BlueprintCallable)
		void AirBalance();

	UFUNCTION(BlueprintCallable)
		void ActivateTurbo();

		void DectivateTurbo();

	UFUNCTION(BlueprintCallable)
		void Disable();
};
