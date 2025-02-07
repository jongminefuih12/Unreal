// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Text_UI2.generated.h"

class UWidgetComponent;
class UTextBlock;

UCLASS()
class FINALPR_API AText_UI2 : public ACharacter
{
	GENERATED_BODY()

private:
	// 텍스트가 서서히 사라지도록 할 타이머 핸들
	FTimerHandle DisappearTimerHandle;
	FTimerHandle FadeTimerHandle;

	// Opacity를 서서히 줄여주는 변수
	float CurrentOpacity = 1.0f;
	const float FadeDuration = 3.0f; // 서서히 사라지는 시간 (3초)
	const float FadeStep = 0.01f; // 한 번에 줄어드는 값

public:
	// Sets default values for this character's properties
	AText_UI2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* widget_txt;

	// 텍스트 UI 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	AText_UI2* TextUIActor2; // AText_UI 포인터

	void UpdateText(FText NewText);
};
