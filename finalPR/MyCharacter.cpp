// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"  
#include "Engine/World.h"  
#include "Animation/AnimInstance.h"  
#include "Animation/AnimMontage.h"  
#include "MyAnimInstance.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Bell.h"
#include "MyGameInstance.h"
#include "Text_UI.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsMovingForward = false;

	// 기본 사운드 컴포넌트 생성
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결

	// 걷기 소리 로드
	static ConstructorHelpers::FObjectFinder<USoundBase> WalkingSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/WalkingSound.WalkingSound'"));
	if (WalkingSoundAsset.Succeeded())
	{
		WalkingSound = WalkingSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> RunningSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/RunningSound.RunningSound'"));
	if (RunningSoundAsset.Succeeded())
	{
		RunningSound = RunningSoundAsset.Object;
	}

	// 이동 속도 초기화
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; // 기본 걷기 속도
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f; // 웅크린 상태 속도

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("arm"));
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("cam"));

	ConstructorHelpers::FClassFinder<UAnimInstance> animClass(TEXT("/Script/Engine.AnimBlueprint'/Game/BP/MyCharacter_BP.MyCharacter_BP_C'"));
	if (animClass.Succeeded())
	{
		this->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		this->GetMesh()->AnimClass = animClass.Class;

	}

	arm->SetupAttachment(RootComponent);
	cam->SetupAttachment(arm);

	arm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	arm->TargetArmLength = -22.0f;

	cam->bUsePawnControlRotation = false; // 카메라는 직접 회전하지 않음
	bUseControllerRotationPitch = true; // 캐릭터의 Pitch 회전을 방지
	bUseControllerRotationYaw = true; // 캐릭터의 Yaw 회전 허용
	bUseControllerRotationRoll = false; // Roll 회전 방지

	// 피치 각도 제한 범위 설정 (기본 범위는 -80 ~ 80)
	PitchMin = -45.0f;
	PitchMax = 45.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_1.SKM_Soldier_1'"));

	if (skeletal.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletal.Object);
		this->GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
		this->GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 흔들림 초기값 설정
	TimeElapsed = 0.0f;
	BobFrequency = 10.0f; // 흔들림 주기 (높을수록 빠름)
	BobAmplitude = 4.5f; // 흔들림 진폭 (높을수록 강함)
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	rotDest = this->GetActorRotation();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 흔들림 효과 계산
	if (GetVelocity().Size() > 0) // 캐릭터가 움직이고 있을 때만 흔들림 적용
	{
		TimeElapsed += DeltaTime;

		// Sine 함수를 사용해 위아래 흔들림 계산
		float VerticalOffset = FMath::Sin(TimeElapsed * BobFrequency) * BobAmplitude;

		// SpringArm의 SocketOffset 조정
		FVector NewSocketOffset = arm->SocketOffset;
		NewSocketOffset.Z = 100.0f + VerticalOffset; // 원래 높이에 흔들림 추가
		arm->SocketOffset = NewSocketOffset;
	}
	else
	{
		// 캐릭터가 정지하면 흔들림 초기화
		TimeElapsed = 0.0f;
		arm->SocketOffset = FVector(0.0f, 0.0f, 100.0f); // 기본 높이로 복구

		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
		}
	}

}

// 달리기 시작 함수 수정
void AMyCharacter::StartRunning()
{
	if (bIsMovingForward) // W 키가 눌려있는 경우에만 달리기 가능
	{
		// 달리기 속도로 설정
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		BobFrequency = 20.0f;
		BobAmplitude = 8.5f;

		// 달리기 애니메이션 활성화
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = false; // 달리기 애니메이션 활성화
		}

		// 달리기 소리 재생
		if (RunningSound && AudioComponent)
		{
			if (AudioComponent->IsPlaying())
			{
				AudioComponent->Stop(); // 현재 재생 중인 소리 중지
			}

			AudioComponent->SetSound(RunningSound);  // 달리기 사운드 설정
			AudioComponent->Play();  // 달리기 소리 재생
			AudioComponent->SetPitchMultiplier(1.0f); // 속도에 맞는 피치 설정
		}
	}
}

// 달리기 중지 함수 (변경 없음)
void AMyCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; // 걷기 속도로 복귀
	BobFrequency = 10.0f;
	BobAmplitude = 6.5f;

	// 걷기 애니메이션 복귀
	UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->isBlend = true; // 걷기 애니메이션 활성화
	}

	// 걷기 소리 재생
	if (WalkingSound && AudioComponent && isWalking)
	{
		if (AudioComponent->IsPlaying())
		{
			AudioComponent->Stop(); // 현재 재생 중인 소리 중지
		}

		AudioComponent->SetSound(WalkingSound);  // 걷기 사운드 설정
		AudioComponent->Play();  // 걷기 소리 재생
		AudioComponent->SetPitchMultiplier(0.74f); // 속도에 맞는 피치 설정
	}
}

//void AMyCharacter::GoToTargetMap()
//{
//	// AText_UI 인스턴스 찾기
//	TArray<AActor*> FoundActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AText_UI::StaticClass(), FoundActors);
//
//	if (FoundActors.Num() > 0)
//	{
//		// 첫 번째 AText_UI 액터에 대해 텍스트 초기화
//		AText_UI* TextUIActor = Cast<AText_UI>(FoundActors[0]);
//		if (TextUIActor)
//		{
//			// 텍스트 초기화
//			TextUIActor->UpdateText(FText::FromString(TEXT("00:00")));
//		}
//	}
//
//	// 맵 이름 설정
//	FName TargetMapName = TEXT("StartScene"); // 원하는 맵 이름으로 변경
//
//	// 맵 이동
//	UGameplayStatics::OpenLevel(GetWorld(), TargetMapName);
//}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("Horizontal", this, &AMyCharacter::horizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMyCharacter::vertical);
	PlayerInputComponent->BindAxis("Yaw", this, &AMyCharacter::yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AMyCharacter::pitch);

	PlayerInputComponent->BindAction("W", EInputEvent::IE_Pressed, this, &AMyCharacter::StartWalking);
	PlayerInputComponent->BindAction("W", EInputEvent::IE_Released, this, &AMyCharacter::StopWalking);

	// 달리기와 걷기 전환
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyCharacter::StopRunning);
}

void AMyCharacter::PlayWalkingSound()
{
	if (AudioComponent && WalkingSound)
	{
		
		// 종 소리 설정하고 재생
		AudioComponent->SetSound(WalkingSound);  // 종 소리 설정
		AudioComponent->Play();  // 종 소리 재생
	}
}

void AMyCharacter::StartWalking()
{
	if (!isWalking)
	{
		isWalking = true;
		bIsMovingForward = true; // W 키 눌림 상태로 설정

		// 걷기 애니메이션 활성화
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = true; // 걷기 애니메이션 활성화
		}

		// 걷기 소리 재생
		if (WalkingSound && AudioComponent)
		{
			if (AudioComponent->IsPlaying())
			{
				AudioComponent->Stop(); // 현재 재생 중인 소리 중지
			}

			AudioComponent->SetSound(WalkingSound);  // 걷기 사운드 설정
			AudioComponent->Play();  // 걷기 소리 재생
			AudioComponent->SetPitchMultiplier(0.74f); // 속도에 맞는 피치 설정
		}
	}
}

// 걷기 중지 함수 수정
void AMyCharacter::StopWalking()
{
	if (isWalking)
	{
		isWalking = false;
		bIsMovingForward = false; // W 키 눌림 상태 해제

		// 걷기 애니메이션 비활성화
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = false; // 걷기 애니메이션 비활성화
		}

		// 걷기 소리 중지
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop(); // 걷기 소리 중지
		}
	}
}

void AMyCharacter::vertical(float v)
{

	 // W 키에만 반응하도록 수정
	if (v > 0.0f)  // W 키가 눌렸을 때만 앞쪽으로 이동
	{
		this->AddMovementInput(this->GetActorForwardVector(), v);
	}
}

void AMyCharacter::yaw(float v)
{
	this->AddControllerYawInput(v);
}

void AMyCharacter::pitch(float v)
{
	
	this->AddControllerPitchInput(-v);
}

void AMyCharacter::animation()
{
	UMyAnimInstance* AnimInst = Cast< UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
	AnimInst->isBlend = !AnimInst->isBlend;
}



