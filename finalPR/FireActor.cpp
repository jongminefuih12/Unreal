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
	AudioComponent->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결
	AudioComponent->SetIntParameter(TEXT("Priority"), 2);

	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/FireSound.FireSound'"));
	if (FireSoundAsset.Succeeded())
	{
		FireSound = FireSoundAsset.Object;
	}

	// 불의 시각적 표현을 위한 StaticMeshComponent 생성
	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
	RootComponent = FireMesh;

	EffectRadius = 700.0f;
	MaxScale = 7.0f;       // 최대 크기
	MinScale = 1.0f;       // 최소 크기
}

void AFireActor::PlayFireSound()
{
	if (AudioComponent && FireSound)
	{
		// 소리 설정 후 활성화 및 재생
		AudioComponent->SetSound(FireSound);  // 사운드 설정
		if (!AudioComponent->IsPlaying())  // 이미 소리가 재생 중이 아닌 경우에만 재생
		{
			AudioComponent->Play();  // 소리 재생
		}
	}
}

void AFireActor::StopFireSound()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();  // 불 소리 중지
		AudioComponent->SetSound(nullptr);
	}
}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 캐릭터 참조 가져오기
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerCharacter)
	{
		// 캐릭터와 불의 거리 계산
		float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

		// 불 크기 업데이트
		UpdateFireScale(Distance);

		 // 일정 거리 이내일 때만 소리 재생
		if (Distance <= EffectRadius)  // 가까워졌을 때
		{
			// 소리가 아직 재생 중이 아니면 재생
			if (!AudioComponent->IsPlaying())
			{
				PlayFireSound();  // 소리 재생
			}
		}
		else  // 일정 거리 이상 멀어졌을 때
		{
			// 소리가 재생 중일 경우에만 중지
			if (AudioComponent->IsPlaying())
			{
				StopFireSound();  // 소리 중지
			}
		}
	}
}

void AFireActor::UpdateFireScale(float Distance)
{
	// 거리 기반으로 크기 계산
	float ScaleFactor = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, EffectRadius), // 거리 범위
		FVector2D(MaxScale, MinScale), // 크기 범위
		Distance                       // 현재 거리
	);

	// 크기 업데이트
	SetActorScale3D(FVector(ScaleFactor));
}

