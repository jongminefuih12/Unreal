// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowAnimInstance.h"
#include "FollowSoldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

void UFollowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // 캐릭터 유효성 검사
    if (!FollowSoldier.IsValid())
    { 
        FollowSoldier = Cast<AFollowSoldier>(TryGetPawnOwner());
    }

    if (FollowSoldier.IsValid())
    {
        // 캐릭터 상태 가져오기
        Speed = FollowSoldier->GetVelocity().Size();
    }
}

void UFollowAnimInstance::PlayMontage(UAnimMontage* MontageToPlay)
{
    if (MontageToPlay && !Montage_IsPlaying(MontageToPlay))
    {
        Montage_Play(MontageToPlay, 1.0f);
    }
}
