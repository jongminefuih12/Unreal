// StartWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

class AStartSceneActor;
class UButton;  // 버튼 클래스를 포함

UCLASS()
class FINALPR_API UStartWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 버튼 클릭 이벤트 함수
    UFUNCTION()
    void OnStartButtonClicked();

protected:
    // 위젯이 화면에 추가될 때 호출되는 함수
    virtual void NativeConstruct() override;

private:
    // 버튼 클릭을 위한 UButton 포인터
    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton;  // StartButton은 블루프린트에서 바인딩된 UButton 컴포넌트
};