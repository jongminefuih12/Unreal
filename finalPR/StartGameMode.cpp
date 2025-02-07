// StartGameMode.cpp
#include "StartGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AStartGameMode::AStartGameMode()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/StartWidget_BP.StartWidget_BP_C"));
    if (WidgetClassFinder.Succeeded())
    {
        StartWidget_BP = WidgetClassFinder.Class;
    }
    // 기본 게임 모드 설정
    // 게임 시작 시, 위젯을 화면에 추가할 수 있게끔 설정
}

void AStartGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartWidget->AddToViewport();

}