// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleClass.h"
#include "WheeledVehicleMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Components/AudioComponent.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

AVehicleClass::AVehicleClass()
{
	ConstructorHelpers::FObjectFinder<USoundBase> EngineSound(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue"));
	ConstructorHelpers::FObjectFinder<USoundAttenuation> EngineSoundAttenuation(TEXT("/Game/VehicleAdv/Sound/Engine_att"));
	
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(GetMesh());
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(GetMesh());
	AudioComponent->SetSound(EngineSound.Object);
	AudioComponent->AttenuationSettings = EngineSoundAttenuation.Object;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->SetRelativeRotation(FRotator( -20.0f, 0.0f,  0.0f));
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 35.0f));
	DefaultCameraRotation = SpringArmComponent->GetComponentRotation();

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(SpringArmComponent);
	FollowCamera->SetRelativeLocation(FVector(-125.0f, 0.0f, 0.0f));
	FollowCamera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));

	Wheel1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wheel1"));
	Wheel1->SetupAttachment(GetMesh(), FName("wheel_front_leftSocket"));

	Wheel2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wheel2"));
	Wheel2->SetupAttachment(GetMesh(), FName("wheel_front_rightSocket"));

	Wheel3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wheel3"));
	Wheel3->SetupAttachment(GetMesh(), FName("wheel_rear_leftSocket"));

	Wheel4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wheel4"));
	Wheel4->SetupAttachment(GetMesh(), FName("wheel_rear_rightSocket"));
}

void AVehicleClass::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicleClass::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PlayAudio();
	//AirBalance();

	FreeCameraLook();
}

void AVehicleClass::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AVehicleClass::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVehicleClass::MoveRight);
	InputComponent->BindAxis("Brake", this, &AVehicleClass::Brake);
	InputComponent->BindAxis("LookRight");
	InputComponent->BindAxis("LookUp");

	InputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehicleClass::HandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &AVehicleClass::HandbrakeReleased);
}

void AVehicleClass::MoveForward(float value)
{
	GetVehicleMovement()->SetThrottleInput(value);
}

void AVehicleClass::MoveRight(float value)
{
	GetVehicleMovement()->SetSteeringInput(value);
}

void AVehicleClass::Brake(float value)
{
	if (value >= 0.2f)
	{
		GetVehicleMovement()->bReverseAsBrake = false;
	}
	else
	{
		GetVehicleMovement()->bReverseAsBrake = true;
	}

	GetVehicleMovement()->SetBrakeInput(value);
}

void AVehicleClass::HandbrakePressed()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void AVehicleClass::HandbrakeReleased()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void AVehicleClass::PlayAudio()
{
	float Speed;

	Speed = FMath::Abs(GetVehicleMovement()->GetForwardSpeed());

	AudioComponent->SetFloatParameter(FName("RPM"), Speed);
}

void AVehicleClass::FreeCameraLook()
{
	//float Pitch = InputComponent->GetAxisValue("LookUp");
	//float Yaw = InputComponent->GetAxisValue("LookRight");
	
	//
	//if(Pitch > 0 || Yaw > 0)
	//{
	//	SpringArmComponent->AddRelativeRotation(FRotator(Pitch, Yaw, 0.0f));
	//}
	//else
	//{
	//	SpringArmComponent->SetRelativeRotation(DefaultCameraRotation, true);
	//}
}

void AVehicleClass::SetReverseBrake(bool b)
{
	GetVehicleMovement()->bReverseAsBrake = b;
}

void AVehicleClass::AirBalance()
{
	FHitResult OutHit;
	UPrimitiveComponent* Primitive;
	FVector CurrentVelocity;
	FTransform Transform;

	Primitive = GetVehicleMovement()->UpdatedPrimitive;
	Transform = Primitive->GetRelativeTransform();

	GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation() + 10 * GetActorUpVector(), GetActorLocation() - 60 * GetActorUpVector(), ECollisionChannel::ECC_Visibility);

	if(!OutHit.bBlockingHit)
	{
		CurrentVelocity = Transform.Rotator().RotateVector(FVector(0.0f, GetActorRotation().Pitch, 0.0f));
		Primitive->SetPhysicsAngularVelocity(CurrentVelocity, true);
	}
}

void AVehicleClass::ActivateTurbo()
{
	if (!IsTurboActivated)
	{
		GetVehicleMovement()->UpdatedPrimitive->SetPhysicsLinearVelocity(GetActorForwardVector() * 500, true);

		IsTurboActivated = true;
		
		GetWorldTimerManager().SetTimer(TurboHandle, this, &AVehicleClass::DectivateTurbo, 3.0f);
	}
}

void AVehicleClass::DectivateTurbo()
{
	GetVehicleMovement()->UpdatedPrimitive->SetPhysicsLinearVelocity(- GetActorForwardVector() * 500, true);

	IsTurboActivated = false;
}

void AVehicleClass::Disable()
{
	GetMesh()->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);
	GetVehicleMovement()->StopActiveMovement();
	AudioComponent->Stop();
}


