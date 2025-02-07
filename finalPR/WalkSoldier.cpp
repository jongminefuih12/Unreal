// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkSoldier.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GhostCharacter.h"
#include "Animation/AnimInstance.h"  
#include "Animation/AnimMontage.h"  

AWalkSoldier::AWalkSoldier() : Super()
{
}

void AWalkSoldier::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AWalkSoldier::repeatTick, 1.0f, true, 1.0f);
}

void AWalkSoldier::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

void AWalkSoldier::repeatTick()
{
    auto navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (navSystem != nullptr && this->GetPawn() != nullptr)
    {
        FNavLocation navLocation;

        // NavMesh 반경 내에서 랜덤한 위치 가져오기
        bool found = navSystem->GetRandomPointInNavigableRadius(
            this->GetPawn()->GetActorLocation(), 1000, navLocation);

        if (found)
        {
            // 플레이어 캐릭터 가져오기
            ACharacter* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (PlayerActor)
            {
                FVector playerLocation = PlayerActor->GetActorLocation();
                FVector pawnLocation = this->GetPawn()->GetActorLocation();

                // 플레이어와의 방향 계산
                FVector dir = playerLocation - pawnLocation;
                dir.Normalize();

                // NavMesh에서 랜덤 위치를 기반으로 이동 (플레이어 위치 기반 랜덤 목표 추가 가능)
                FVector targetLocation = navLocation.Location;

                // AIController를 통해 이동 명령 전달
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, targetLocation);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("AWalkSoldier::repeatTick"));
}
