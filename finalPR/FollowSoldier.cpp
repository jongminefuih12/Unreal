// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowSoldier.h"
#include "AIFollowSildier.h"
#include "Animation/AnimInstance.h"  
#include "Animation/AnimMontage.h"  
#include "GameFramework/CharacterMovementComponent.h"
#include "FollowAnimInstance.h"

// Sets default values
AFollowSoldier::AFollowSoldier()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = AAIFollowSildier::StaticClass();
	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AFollowSoldier::BeginPlay()
{
	Super::BeginPlay();

	//PlayFollowMontage();
}

void AFollowSoldier::PlayFollowMontage()
{
}

void AFollowSoldier::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

// Called every frame
void AFollowSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFollowSoldier::SetIsBlend(bool bNewState)
{
}

void AFollowSoldier::TriggerMontage()
{
}

void AFollowSoldier::PlayExampleMontage()
{
}

// Called to bind functionality to input
void AFollowSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
