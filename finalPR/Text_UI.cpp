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
	// 3�� �� �ؽ�Ʈ ������ ��������� ����
    GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &AText_UI::StartFadeOut, 0.01f, false);

    // GameInstance���� �ؽ�Ʈ ��������
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance && !GameInstance->SavedText.IsEmpty())
    {
        // GameInstance�� �ؽ�Ʈ�� ������Ʈ
        UpdateText(GameInstance->SavedText);
    }
    else
    {
        // GameInstance�� �ؽ�Ʈ�� ���ٸ� �⺻ �ؽ�Ʈ ����
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
                TextBlock->SetText(NewText); // ���ο� �ؽ�Ʈ�� ������Ʈ
            }
        }
    }
}

// ������ ������� �ϴ� �Լ�
void AText_UI::StartFadeOut()
{
    if (widget_txt)
    {
        // ������ �ؽ�Ʈ�� ���� (�ڽ��� ���� ������ ���� ���)
        UUserWidget* UserWidget = widget_txt->GetUserWidgetObject();
        if (UserWidget)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("MyTextBlock")));
            if (TextBlock)
            {
                // �ؽ�Ʈ ����� ���� ����
                FadeTextOut(TextBlock);
            }
        }
    }
}

// �ؽ�Ʈ�� opacity�� ������ �ٿ����� �Լ�
void AText_UI::FadeTextOut(UTextBlock* TextBlock)
{
    if (TextBlock)
    {
        // �ؽ�Ʈ�� ������ ��������� 0.1�� �������� Ÿ�̸� ȣ��
        GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, [this, TextBlock]()
            {
                UpdateOpacity(TextBlock);
            }, 0.025f, true);
    }
}


// �ؽ�Ʈ opacity�� ������Ʈ�ϴ� �Լ�
void AText_UI::UpdateOpacity(UTextBlock* TextBlock)
{
    if (TextBlock && CurrentOpacity > 0.0f)
    {
        // ���� opacity �� ��� (���� opacity ������ �������� ����)
        CurrentOpacity -= FadeStep;

        // �ؽ�Ʈ ��Ͽ� ����
        TextBlock->SetOpacity(CurrentOpacity);

        // opacity�� 0�� �����ϸ� Ÿ�̸Ӹ� �����ϰ� �ؽ�Ʈ�� ����
        if (CurrentOpacity <= 0.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
            TextBlock->SetVisibility(ESlateVisibility::Hidden); // �����
        }
    }
}

// Called every frame
void AText_UI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    
}



