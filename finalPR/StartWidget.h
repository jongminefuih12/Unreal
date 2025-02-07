// StartWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

class AStartSceneActor;
class UButton;  // ��ư Ŭ������ ����

UCLASS()
class FINALPR_API UStartWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // ��ư Ŭ�� �̺�Ʈ �Լ�
    UFUNCTION()
    void OnStartButtonClicked();

protected:
    // ������ ȭ�鿡 �߰��� �� ȣ��Ǵ� �Լ�
    virtual void NativeConstruct() override;

private:
    // ��ư Ŭ���� ���� UButton ������
    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton;  // StartButton�� �������Ʈ���� ���ε��� UButton ������Ʈ
};