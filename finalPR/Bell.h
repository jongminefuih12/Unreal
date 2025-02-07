// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bell.generated.h"

class AText_UI;

UCLASS()
class FINALPR_API ABell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABell();

private:
	int32 CurrentHour = 0;

	FTimerHandle TimerHandle;  // 타이머 핸들 추가

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetAudioComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* BellSound;

	// 텍스트 UI 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	AText_UI* TextUIActor; // AText_UI 포인터

	UPROPERTY()
	class UCapsuleComponent* capsule;

	void PlayBellSound();

	void ResetText();

	UFUNCTION()
	void player_overlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

};
