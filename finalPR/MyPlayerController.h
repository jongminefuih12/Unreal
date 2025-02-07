// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" // UUserWidget ����� ���� �ʿ�
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

    // UI Ŭ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> UIWidgetClass;

    AMyPlayerController();

private:
    // ������ ���� �ν��Ͻ�
    UPROPERTY()
    UUserWidget* UIWidget;

    // �ؽ�Ʈ ������Ʈ �Լ�
    void UpdateUIText(const FString& NewText);
};
