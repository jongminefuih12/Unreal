// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::SetText(const FString& NewText)
{
    if (TextBlock)
    {
        TextBlock->SetText(FText::FromString(NewText));
    }
}