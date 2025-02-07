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
    // �⺻ ���� ������Ʈ ����
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����

    // �ȱ� �Ҹ� �ε� (���� ���� ��ο� �°� ����)
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
            // 1. �÷��̾�� AI �� ���� ���� ���
            FVector dir = PlayerActor->GetActorLocation() - this->GetPawn()->GetActorLocation();
            dir.Z = 0; // Z�� ���� (���� ���⸸ ���)
            dir.Normalize(); // ����ȭ�Ͽ� ���⸸ ���

            // 2. AI �̵� ó��
            this->GetPawn()->SetActorLocation(this->GetPawn()->GetActorLocation() + dir * 80);

            // 3. �̵� ������ �������� ȸ�� ó��
            FRotator TargetRotation = dir.Rotation(); // ���� ���͸� �������� ȸ���� ���
            TargetRotation.Pitch = 0.0f; // Pitch ���� (���� ����)
            TargetRotation.Roll = 0.0f;  // Roll ���� (���� ����)

            // 4. ���� ȸ���� ���� ���� �ӵ��� ���� (20.0f�� ����)
            FRotator CurrentRotation = this->GetPawn()->GetActorRotation();
            FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f); // 20.0f: ȸ�� �ӵ�

            // 5. ȸ�� ����
            this->GetPawn()->SetActorRotation(SmoothRotation);

            if (AudioComponent && WalkingSound)
            {
                
                AudioComponent->SetSound(WalkingSound);  // �޸��� ���� ����
                AudioComponent->Play();  // �޸��� �Ҹ� ���
                AudioComponent->SetPitchMultiplier(1.0f);  // �ӵ��� �´� ��ġ ����
            }
        }
    }
}

