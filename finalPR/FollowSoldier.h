// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FollowSoldier.generated.h"

UCLASS()
class FINALPR_API AFollowSoldier : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFollowSoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

private:
	// 몽타주 반복 실행 함수
	void PlayFollowMontage();

	// 콜백: 몽타주 끝날 때 호출
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsBlend(bool bNewState);

	void TriggerMontage();

	void PlayExampleMontage();

	// 애니메이션 상태를 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsBlend;

	// FollowMontage 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FollowMontage;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
