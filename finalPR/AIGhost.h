// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIGhost.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API AAIGhost : public AAIController
{
	GENERATED_BODY()

public:
	AAIGhost();
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle timerHandle;

	APawn* thisPawn;
	void repeatTick();

};