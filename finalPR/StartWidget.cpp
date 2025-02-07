// StartWidget.cpp
#include "StartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UStartWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư�� ȭ�鿡�� ã�� Ŭ�� �̺�Ʈ ó��
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UStartWidget::OnStartButtonClicked);
    }
}

void UStartWidget::OnStartButtonClicked()
{
    // "FirstScene" ������ ��ȯ
    UGameplayStatics::OpenLevel(this, FName(TEXT("FirstScene")));
}