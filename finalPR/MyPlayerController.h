// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" // UUserWidget 사용을 위해 필요
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

    // UI 클래스 참조
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> UIWidgetClass;

    AMyPlayerController();

private:
    // 생성된 위젯 인스턴스
    UPROPERTY()
    UUserWidget* UIWidget;

    // 텍스트 업데이트 함수
    void UpdateUIText(const FString& NewText);
};
