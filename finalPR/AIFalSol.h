// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIFalSol.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API AAIFalSol : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	USoundBase* WalkingSound;

public:
	AAIFalSol();
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle timerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;

	void repeatTick();
};
