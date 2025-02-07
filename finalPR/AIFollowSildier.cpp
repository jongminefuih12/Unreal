// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFollowSildier.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GhostCharacter.h"
#include "Animation/AnimInstance.h"  
#include "Animation/AnimMontage.h"  

AAIFollowSildier::AAIFollowSildier() : Super()
{
}

void AAIFollowSildier::OnPossess(APawn* pawn)
{
    //Super::OnPossess(pawn);

    GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAIFollowSildier::repeatTick, 0.5f, true, 0.5f);
}

void AAIFollowSildier::OnUnPossess()
{
   // Super::OnUnPossess();

    GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

void AAIFollowSildier::repeatTick()
{
	auto navLocation = new FNavLocation();

	auto navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (navSystem != nullptr && this->GetPawn() != nullptr)
	{
		navSystem->GetRandomPointInNavigableRadius(
			this->GetPawn()->GetActorLocation(),
			1000, *navLocation);

		auto PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FVector dir = PlayerActor->GetActorLocation() - this->GetPawn()->GetActorLocation();


		this->GetPawn()->SetActorLocation(navLocation->Location);
		//this->GetPawn()->SetActorLocation(this->GetPawn()->GetActorLocation() + dir.GetSafeNormal() * 20);

	}	UE_LOG(LogTemp, Log, TEXT("AIFollowSildier::repeatTick"));
}
