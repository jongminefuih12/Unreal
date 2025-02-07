// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class FINALPR_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	FRotator rotDest;

	void GoToTargetMap();

	bool bIsMovingForward;

	float TimeElapsed; // ��� �ð�
	float BobFrequency; // ��鸲 �ֱ�
	float BobAmplitude; // ��鸲 ����

	UPROPERTY(EditAnywhere)
	USoundBase* WalkingSound;   // �ȱ� �Ҹ�

	UPROPERTY(EditAnywhere)
	USoundBase* RunningSound;


public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// �ȱ� �ִϸ��̼� ��ȯ �Լ�
	void StartWalking();
	void StopWalking();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayWalkingSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;  // ����� ������Ʈ ����


	UPROPERTY()
	float verticalValue = 0;
	UPROPERTY()
	float horizontalValue = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	bool isWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* widget_txt;

	UPROPERTY()
	class USpringArmComponent* arm;

	UPROPERTY()
	class UCameraComponent* cam;

	UFUNCTION()
	void vertical(float v);
	UFUNCTION()
	void yaw(float v);
	UFUNCTION()
	void pitch(float v);
	UFUNCTION()
	void animation();

	float PitchMin;
	float PitchMax;

	void StopRunning();

	void StartRunning();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Direction;
};
