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
    // TextBlock ���� ����
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextBlock; // UI���� ����� �ؽ�Ʈ ���

    // �ؽ�Ʈ ���� �Լ�
    void SetText(const FString& NewText);
};
