// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Text_UI.generated.h"

class UWidgetComponent;
class UTextBlock;

UCLASS()
class FINALPR_API AText_UI : public ACharacter
{
	GENERATED_BODY()

private:
	// �ؽ�Ʈ�� ������ ��������� �� Ÿ�̸� �ڵ�
	FTimerHandle DisappearTimerHandle;
	FTimerHandle FadeTimerHandle;

	// Opacity�� ������ �ٿ��ִ� ����
	float CurrentOpacity = 1.0f;
	const float FadeDuration = 3.0f; // ������ ������� �ð� (3��)
	const float FadeStep = 0.01f; // �� ���� �پ��� ��

public:
	// Sets default values for this character's properties
	AText_UI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* widget_txt;

	// �ؽ�Ʈ UI ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	AText_UI* TextUIActor; // AText_UI ������

	// �ؽ�Ʈ ������� �����ϴ� �Լ�
	void StartFadeOut();

	// �ؽ�Ʈ�� opacity�� ������ �ٿ��ִ� �Լ�
	void FadeTextOut(UTextBlock* TextBlock);

	// �ؽ�Ʈ opacity�� ������Ʈ�ϴ� �Լ�
	void UpdateOpacity(UTextBlock* TextBlock);

	void UpdateText(FText NewText);
};
