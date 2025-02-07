// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // TextBlock 변수 연결
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextBlock; // UI에서 사용할 텍스트 블록

    // 텍스트 설정 함수
    void SetText(const FString& NewText);
};
