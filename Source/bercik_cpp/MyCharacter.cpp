// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	static ConstructorHelpers::FObjectFinder<UClass> BlueprintBercik(TEXT("Blueprint'/Game/Blueprints/BP_Bercik.BP_Bercik_C'"));
	if (BlueprintBercik.Object) {
		SubBercik = (UClass*)BlueprintBercik.Object;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	BercikTimer = 0;
	BercikIsSpawned = false;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AMyCharacter::SpawnOrDestroyBercik(int GainedBieluchs = 0)
{
	if (BercikIsSpawned == false && BercikTimer >= (45 - 5 * GainedBieluchs) * 10)
	{
		BercikIsSpawned = true;
		float tick = GetActorTickInterval();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotator = GetActorRotation();
		rotator.Pitch = 0.0;
		rotator.Roll = 0.0;
		FVector location = GetActorLocation();
		location += GetActorForwardVector() * (500 - GainedBieluchs * 10);
		location.Z = 0.0;
		UWorld* world = GetWorld();
		CPP_Bercik = world->SpawnActor<ABercik>(SubBercik, location, rotator, spawnParams);
		BercikTimer = 0;
	}
	else if (BercikIsSpawned == false && BercikTimer >= 0)
	{
		BercikTimer++;
	}
	else
	{
		FVector PlayerLoc = GetActorLocation();
		FVector BercikLoc = CPP_Bercik->GetActorLocation();
		if ((UKismetMathLibrary::Distance2D({ PlayerLoc.X, PlayerLoc.Y }, { BercikLoc.X, BercikLoc.Y }) <= 100 - 10 * GainedBieluchs) || BercikTimer >= 350)
		{
			CPP_Bercik->Destroy();
			BercikIsSpawned = false;
			BercikTimer = 0;
		}
		else
		{
			BercikTimer++;
			CPP_Bercik->Move(PlayerLoc);
		}
	}
}
