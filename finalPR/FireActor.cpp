// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 


// Sets default values
AFireActor::AFireActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����
	AudioComponent->SetIntParameter(TEXT("Priority"), 2);

	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/FireSound.FireSound'"));
	if (FireSoundAsset.Succeeded())
	{
		FireSound = FireSoundAsset.Object;
	}

	// ���� �ð��� ǥ���� ���� StaticMeshComponent ����
	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
	RootComponent = FireMesh;

	EffectRadius = 700.0f;
	MaxScale = 7.0f;       // �ִ� ũ��
	MinScale = 1.0f;       // �ּ� ũ��
}

void AFireActor::PlayFireSound()
{
	if (AudioComponent && FireSound)
	{
		// �Ҹ� ���� �� Ȱ��ȭ �� ���
		AudioComponent->SetSound(FireSound);  // ���� ����
		if (!AudioComponent->IsPlaying())  // �̹� �Ҹ��� ��� ���� �ƴ� ��쿡�� ���
		{
			AudioComponent->Play();  // �Ҹ� ���
		}
	}
}

void AFireActor::StopFireSound()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();  // �� �Ҹ� ����
		AudioComponent->SetSound(nullptr);
	}
}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
	Super::BeginPlay();
	
	// �÷��̾� ĳ���� ���� ��������
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerCharacter)
	{
		// ĳ���Ϳ� ���� �Ÿ� ���
		float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

		// �� ũ�� ������Ʈ
		UpdateFireScale(Distance);

		 // ���� �Ÿ� �̳��� ���� �Ҹ� ���
		if (Distance <= EffectRadius)  // ��������� ��
		{
			// �Ҹ��� ���� ��� ���� �ƴϸ� ���
			if (!AudioComponent->IsPlaying())
			{
				PlayFireSound();  // �Ҹ� ���
			}
		}
		else  // ���� �Ÿ� �̻� �־����� ��
		{
			// �Ҹ��� ��� ���� ��쿡�� ����
			if (AudioComponent->IsPlaying())
			{
				StopFireSound();  // �Ҹ� ����
			}
		}
	}
}

void AFireActor::UpdateFireScale(float Distance)
{
	// �Ÿ� ������� ũ�� ���
	float ScaleFactor = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, EffectRadius), // �Ÿ� ����
		FVector2D(MaxScale, MinScale), // ũ�� ����
		Distance                       // ���� �Ÿ�
	);

	// ũ�� ������Ʈ
	SetActorScale3D(FVector(ScaleFactor));
}

