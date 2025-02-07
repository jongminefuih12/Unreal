// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGhost.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GhostCharacter.h"

AAIGhost::AAIGhost() : Super()
{
}

void AAIGhost::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAIGhost::repeatTick, 0.5f, true, 0.5f);
}

void AAIGhost::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}


void AAIGhost::repeatTick()
{
	auto navLocation = new FNavLocation();

	auto navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (navSystem != nullptr && this->GetPawn() != nullptr)
	{
		navSystem->GetRandomPointInNavigableRadius(
			this->GetPawn()->GetActorLocation(),
			1000, *navLocation);

		this->GetPawn()->SetActorLocation(navLocation->Location);

	}
	UE_LOG(LogTemp, Log, TEXT("AAIGhost::repeatTick"));
}
