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
	// ��Ÿ�� �ݺ� ���� �Լ�
	void PlayFollowMontage();

	// �ݹ�: ��Ÿ�� ���� �� ȣ��
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsBlend(bool bNewState);

	void TriggerMontage();

	void PlayExampleMontage();

	// �ִϸ��̼� ���¸� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsBlend;

	// FollowMontage ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FollowMontage;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
