// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameData")
	bool bShouldPlayBellSound; // �� �Ҹ� ��� ����

	// �ؽ�Ʈ �����͸� ������ ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FText SavedText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SavedData")
	FText SavedText_UI2;  // Text_UI2 ���� �ؽ�Ʈ ���� ����

	// �ð� ��
	int32 SavedCurrentHour;
};
