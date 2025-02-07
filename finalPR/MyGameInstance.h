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
	bool bShouldPlayBellSound; // 종 소리 재생 여부

	// 텍스트 데이터를 저장할 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FText SavedText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SavedData")
	FText SavedText_UI2;  // Text_UI2 전용 텍스트 저장 변수

	// 시간 값
	int32 SavedCurrentHour;
};
