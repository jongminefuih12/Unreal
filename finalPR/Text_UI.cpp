// Fill out your copyright notice in the Description page of Project Settings.


#include "Text_UI.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AText_UI::AText_UI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	widget_txt = CreateDefaultSubobject<UWidgetComponent>(TEXT("UITXT"));

	widget_txt->SetupAttachment(this->GetMesh());
}

// Called when the game starts or when spawned
void AText_UI::BeginPlay()
{
	Super::BeginPlay();
	// 3초 후 텍스트 서서히 사라지도록 설정
    GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &AText_UI::StartFadeOut, 0.01f, false);

    // GameInstance에서 텍스트 가져오기
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance && !GameInstance->SavedText.IsEmpty())
    {
        // GameInstance의 텍스트로 업데이트
        UpdateText(GameInstance->SavedText);
    }
    else
    {
        // GameInstance에 텍스트가 없다면 기본 텍스트 설정
        UpdateText(FText::FromString(TEXT("00:00")));
    }
}



void AText_UI::UpdateText(FText NewText)
{
    if (widget_txt)
    {
        UUserWidget* UserWidget = widget_txt->GetUserWidgetObject();
        if (UserWidget)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("MyTextBlock")));
            if (TextBlock)
            {
                TextBlock->SetText(NewText); // 새로운 텍스트로 업데이트
            }
        }
    }
}

// 서서히 사라지게 하는 함수
void AText_UI::StartFadeOut()
{
    if (widget_txt)
    {
        // 위젯의 텍스트에 접근 (자신이 만든 위젯이 있을 경우)
        UUserWidget* UserWidget = widget_txt->GetUserWidgetObject();
        if (UserWidget)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("MyTextBlock")));
            if (TextBlock)
            {
                // 텍스트 블록의 투명도 변경
                FadeTextOut(TextBlock);
            }
        }
    }
}

// 텍스트의 opacity를 서서히 줄여가는 함수
void AText_UI::FadeTextOut(UTextBlock* TextBlock)
{
    if (TextBlock)
    {
        // 텍스트가 서서히 사라지도록 0.1초 간격으로 타이머 호출
        GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, [this, TextBlock]()
            {
                UpdateOpacity(TextBlock);
            }, 0.025f, true);
    }
}


// 텍스트 opacity를 업데이트하는 함수
void AText_UI::UpdateOpacity(UTextBlock* TextBlock)
{
    if (TextBlock && CurrentOpacity > 0.0f)
    {
        // 현재 opacity 값 계산 (현재 opacity 값에서 일정량을 차감)
        CurrentOpacity -= FadeStep;

        // 텍스트 블록에 적용
        TextBlock->SetOpacity(CurrentOpacity);

        // opacity가 0에 도달하면 타이머를 중지하고 텍스트를 숨김
        if (CurrentOpacity <= 0.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
            TextBlock->SetVisibility(ESlateVisibility::Hidden); // 숨기기
        }
    }
}

// Called every frame
void AText_UI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    
}



