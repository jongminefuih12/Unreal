// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayerController.h" // 커스텀 플레이어 컨트롤러 포함
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FINALPR_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	 AMyGameModeBase();
};
