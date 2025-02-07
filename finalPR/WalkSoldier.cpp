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

        // NavMesh �ݰ� ������ ������ ��ġ ��������
        bool found = navSystem->GetRandomPointInNavigableRadius(
            this->GetPawn()->GetActorLocation(), 1000, navLocation);

        if (found)
        {
            // �÷��̾� ĳ���� ��������
            ACharacter* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (PlayerActor)
            {
                FVector playerLocation = PlayerActor->GetActorLocation();
                FVector pawnLocation = this->GetPawn()->GetActorLocation();

                // �÷��̾���� ���� ���
                FVector dir = playerLocation - pawnLocation;
                dir.Normalize();

                // NavMesh���� ���� ��ġ�� ������� �̵� (�÷��̾� ��ġ ��� ���� ��ǥ �߰� ����)
                FVector targetLocation = navLocation.Location;

                // AIController�� ���� �̵� ��� ����
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, targetLocation);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("AWalkSoldier::repeatTick"));
}
