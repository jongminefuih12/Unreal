// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FollowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API UFollowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 몽타주 실행 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayMontage(UAnimMontage* MontageToPlay);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ExampleMontage;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    float Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsBlend;


private:
    TWeakObjectPtr<class AFollowSoldier> FollowSoldier;
};
