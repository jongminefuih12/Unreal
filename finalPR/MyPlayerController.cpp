// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // UTextBlock ����� ���� �ʿ�
#include "MyUserWidget.h"

AMyPlayerController::AMyPlayerController()
{
    // UI_Widget �������Ʈ�� ��θ� �����մϴ�.
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BP/UI_Widget.UI_Widget'"));
    if (WidgetClassFinder.Succeeded())
    {
        UIWidgetClass = WidgetClassFinder.Class;
    }
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // UI ���� ���� �� �߰�
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
        UIWidget->AddToViewport(0); // ����Ʈ �켱���� ����

        // ���� ������ �ؽ�Ʈ ��� ��������
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

