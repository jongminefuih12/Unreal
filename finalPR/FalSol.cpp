// Fill out your copyright notice in the Description page of Project Settings.


#include "FalSol.h"
#include "AIFalSol.h"

// Sets default values
AFalSol::AFalSol()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = AAIFalSol::StaticClass();
	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AFalSol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFalSol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFalSol::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

