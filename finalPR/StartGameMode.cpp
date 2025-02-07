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
    // �⺻ ���� ��� ����
    // ���� ���� ��, ������ ȭ�鿡 �߰��� �� �ְԲ� ����
}

void AStartGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartWidget->AddToViewport();

}