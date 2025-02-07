// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkSoldierCharacter.h"
#include "WalkSoldier.h"
#include "Animation/AnimInstance.h"  
#include "Animation/AnimMontage.h"  

// Sets default values
AWalkSoldierCharacter::AWalkSoldierCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = AWalkSoldier::StaticClass();
	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AWalkSoldierCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWalkSoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWalkSoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

