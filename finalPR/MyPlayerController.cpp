// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // UTextBlock 사용을 위해 필요
#include "MyUserWidget.h"

AMyPlayerController::AMyPlayerController()
{
    // UI_Widget 블루프린트의 경로를 지정합니다.
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BP/UI_Widget.UI_Widget'"));
    if (WidgetClassFinder.Succeeded())
    {
        UIWidgetClass = WidgetClassFinder.Class;
    }
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // UI 위젯 생성 및 추가
    if (UIWidgetClass)
    {
        UIWidget = CreateWidget<UUserWidget>(this, UIWidgetClass);
        if (UIWidget)
        {
            UIWidget->AddToViewport();
        }
    }

}


void AMyPlayerController::UpdateUIText(const FString& NewText)
{
    if (UIWidget)
    {
        UIWidget->AddToViewport(0); // 뷰포트 우선순위 설정

        // 위젯 내부의 텍스트 블록 가져오기
        UTextBlock* TextBlock = Cast<UTextBlock>(UIWidget->GetWidgetFromName(TEXT("MyTextBlock")));
        if (TextBlock)
        {
            TextBlock->SetText(FText::FromString(NewText));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("TextBlock not found!"));
        }
    }
}

