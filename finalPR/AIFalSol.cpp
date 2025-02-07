// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFalSol.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "FalSol.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

AAIFalSol::AAIFalSol() : Super()
{
    // 기본 사운드 컴포넌트 생성
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결

    // 걷기 소리 로드 (사운드 파일 경로에 맞게 수정)
    static ConstructorHelpers::FObjectFinder<USoundBase> WalkingSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/WalkingSound.WalkingSound'"));
    if (WalkingSoundAsset.Succeeded())
    {
        WalkingSound = WalkingSoundAsset.Object;
    }
}

void AAIFalSol::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAIFalSol::repeatTick, 0.5f, true, 0.5f);
}

void AAIFalSol::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

void AAIFalSol::repeatTick()
{
    auto navLocation = new FNavLocation();
    auto navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

    if (navSystem != nullptr && this->GetPawn() != nullptr)
    {
        navSystem->GetRandomPointInNavigableRadius(
            this->GetPawn()->GetActorLocation(),
            1000, *navLocation);

        auto PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerActor)
        {
            // 1. 플레이어와 AI 간 방향 벡터 계산
            FVector dir = PlayerActor->GetActorLocation() - this->GetPawn()->GetActorLocation();
            dir.Z = 0; // Z축 무시 (수평 방향만 계산)
            dir.Normalize(); // 정규화하여 방향만 사용

            // 2. AI 이동 처리
            this->GetPawn()->SetActorLocation(this->GetPawn()->GetActorLocation() + dir * 80);

            // 3. 이동 방향을 기준으로 회전 처리
            FRotator TargetRotation = dir.Rotation(); // 방향 벡터를 기준으로 회전값 계산
            TargetRotation.Pitch = 0.0f; // Pitch 고정 (수평 유지)
            TargetRotation.Roll = 0.0f;  // Roll 고정 (수평 유지)

            // 4. 빠른 회전을 위해 보간 속도를 증가 (20.0f로 설정)
            FRotator CurrentRotation = this->GetPawn()->GetActorRotation();
            FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f); // 20.0f: 회전 속도

            // 5. 회전 적용
            this->GetPawn()->SetActorRotation(SmoothRotation);

            if (AudioComponent && WalkingSound)
            {
                
                AudioComponent->SetSound(WalkingSound);  // 달리기 사운드 설정
                AudioComponent->Play();  // 달리기 소리 재생
                AudioComponent->SetPitchMultiplier(1.0f);  // 속도에 맞는 피치 설정
            }
        }
    }
}

