// Fill out your copyright notice in the Description page of Project Settings.


#include "Text_UI2.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "MyGameInstance.h"

// Sets default values
AText_UI2::AText_UI2()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    widget_txt = CreateDefaultSubobject<UWidgetComponent>(TEXT("UITXT2"));

    widget_txt->SetupAttachment(this->GetMesh());
}

// Called when the game starts or when spawned
void AText_UI2::BeginPlay()
{
    Super::BeginPlay();

    // GameInstance���� Text_UI2 ���� �ؽ�Ʈ ��������
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance && !GameInstance->SavedText_UI2.IsEmpty())
    {
        // GameInstance�� SavedText_UI2�� ������Ʈ
        UpdateText(GameInstance->SavedText_UI2);
    }
    else
    {
        // GameInstance�� Text_UI2 ���� �ؽ�Ʈ�� ���ٸ� �⺻ �ؽ�Ʈ ����
        UpdateText(FText::FromString(TEXT("06:00")));
    }
}

void AText_UI2::UpdateText(FText NewText)
{
    if (widget_txt)
    {
        UUserWidget* UserWidget = widget_txt->GetUserWidgetObject();
        if (UserWidget)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("MyTextBlock2")));
            if (TextBlock)
            {
                TextBlock->SetText(NewText); // ���ο� �ؽ�Ʈ�� ������Ʈ
            }
        }
    }
}

// Called every frame
void AText_UI2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
