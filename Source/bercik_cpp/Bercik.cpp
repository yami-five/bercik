// Fill out your copyright notice in the Description page of Project Settings.


#include "Bercik.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABercik::ABercik()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABercik::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABercik::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABercik::Move(FVector targetLocation)
{
	FVector BercikLocation = GetActorLocation();
	targetLocation *= 1.0;
	FRotator BercikRotator = UKismetMathLibrary::FindLookAtRotation(BercikLocation, targetLocation);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(BercikRotator);
	ForwardVector *= 20;
	ForwardVector = { ForwardVector.X, ForwardVector.Y, 0.0 };
	AddActorWorldOffset(ForwardVector);
	SetActorRotation(UKismetMathLibrary::MakeRotFromX(ForwardVector));
}