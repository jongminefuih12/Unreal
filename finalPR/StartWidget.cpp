// StartWidget.cpp
#include "StartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UStartWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼을 화면에서 찾고 클릭 이벤트 처리
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UStartWidget::OnStartButtonClicked);
    }
}

void UStartWidget::OnStartButtonClicked()
{
    // "FirstScene" 맵으로 전환
    UGameplayStatics::OpenLevel(this, FName(TEXT("FirstScene")));
}