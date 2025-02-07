// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class FINALPR_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AStartGameMode();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> StartWidget_BP; // UUserWidget Ŭ������ �����ϴ� �� ����� Blueprint

    UUserWidget* StartWidget; // ������ ������ ���� ����
};
