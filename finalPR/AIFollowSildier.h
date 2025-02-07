// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIFollowSildier.generated.h"

/**
 *
 */
UCLASS()
class FINALPR_API AAIFollowSildier : public AAIController
{
	GENERATED_BODY()

public:
	AAIFollowSildier();
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle timerHandle;

	void repeatTick();
};