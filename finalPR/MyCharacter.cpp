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

	// �⺻ ���� ������Ʈ ����
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����

	// �ȱ� �Ҹ� �ε�
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

	// �̵� �ӵ� �ʱ�ȭ
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; // �⺻ �ȱ� �ӵ�
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f; // ��ũ�� ���� �ӵ�

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

	cam->bUsePawnControlRotation = false; // ī�޶�� ���� ȸ������ ����
	bUseControllerRotationPitch = true; // ĳ������ Pitch ȸ���� ����
	bUseControllerRotationYaw = true; // ĳ������ Yaw ȸ�� ���
	bUseControllerRotationRoll = false; // Roll ȸ�� ����

	// ��ġ ���� ���� ���� ���� (�⺻ ������ -80 ~ 80)
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

	// ��鸲 �ʱⰪ ����
	TimeElapsed = 0.0f;
	BobFrequency = 10.0f; // ��鸲 �ֱ� (�������� ����)
	BobAmplitude = 4.5f; // ��鸲 ���� (�������� ����)
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


	// ��鸲 ȿ�� ���
	if (GetVelocity().Size() > 0) // ĳ���Ͱ� �����̰� ���� ���� ��鸲 ����
	{
		TimeElapsed += DeltaTime;

		// Sine �Լ��� ����� ���Ʒ� ��鸲 ���
		float VerticalOffset = FMath::Sin(TimeElapsed * BobFrequency) * BobAmplitude;

		// SpringArm�� SocketOffset ����
		FVector NewSocketOffset = arm->SocketOffset;
		NewSocketOffset.Z = 100.0f + VerticalOffset; // ���� ���̿� ��鸲 �߰�
		arm->SocketOffset = NewSocketOffset;
	}
	else
	{
		// ĳ���Ͱ� �����ϸ� ��鸲 �ʱ�ȭ
		TimeElapsed = 0.0f;
		arm->SocketOffset = FVector(0.0f, 0.0f, 100.0f); // �⺻ ���̷� ����

		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
		}
	}

}

// �޸��� ���� �Լ� ����
void AMyCharacter::StartRunning()
{
	if (bIsMovingForward) // W Ű�� �����ִ� ��쿡�� �޸��� ����
	{
		// �޸��� �ӵ��� ����
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		BobFrequency = 20.0f;
		BobAmplitude = 8.5f;

		// �޸��� �ִϸ��̼� Ȱ��ȭ
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = false; // �޸��� �ִϸ��̼� Ȱ��ȭ
		}

		// �޸��� �Ҹ� ���
		if (RunningSound && AudioComponent)
		{
			if (AudioComponent->IsPlaying())
			{
				AudioComponent->Stop(); // ���� ��� ���� �Ҹ� ����
			}

			AudioComponent->SetSound(RunningSound);  // �޸��� ���� ����
			AudioComponent->Play();  // �޸��� �Ҹ� ���
			AudioComponent->SetPitchMultiplier(1.0f); // �ӵ��� �´� ��ġ ����
		}
	}
}

// �޸��� ���� �Լ� (���� ����)
void AMyCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; // �ȱ� �ӵ��� ����
	BobFrequency = 10.0f;
	BobAmplitude = 6.5f;

	// �ȱ� �ִϸ��̼� ����
	UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->isBlend = true; // �ȱ� �ִϸ��̼� Ȱ��ȭ
	}

	// �ȱ� �Ҹ� ���
	if (WalkingSound && AudioComponent && isWalking)
	{
		if (AudioComponent->IsPlaying())
		{
			AudioComponent->Stop(); // ���� ��� ���� �Ҹ� ����
		}

		AudioComponent->SetSound(WalkingSound);  // �ȱ� ���� ����
		AudioComponent->Play();  // �ȱ� �Ҹ� ���
		AudioComponent->SetPitchMultiplier(0.74f); // �ӵ��� �´� ��ġ ����
	}
}

//void AMyCharacter::GoToTargetMap()
//{
//	// AText_UI �ν��Ͻ� ã��
//	TArray<AActor*> FoundActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AText_UI::StaticClass(), FoundActors);
//
//	if (FoundActors.Num() > 0)
//	{
//		// ù ��° AText_UI ���Ϳ� ���� �ؽ�Ʈ �ʱ�ȭ
//		AText_UI* TextUIActor = Cast<AText_UI>(FoundActors[0]);
//		if (TextUIActor)
//		{
//			// �ؽ�Ʈ �ʱ�ȭ
//			TextUIActor->UpdateText(FText::FromString(TEXT("00:00")));
//		}
//	}
//
//	// �� �̸� ����
//	FName TargetMapName = TEXT("StartScene"); // ���ϴ� �� �̸����� ����
//
//	// �� �̵�
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

	// �޸���� �ȱ� ��ȯ
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyCharacter::StopRunning);
}

void AMyCharacter::PlayWalkingSound()
{
	if (AudioComponent && WalkingSound)
	{
		
		// �� �Ҹ� �����ϰ� ���
		AudioComponent->SetSound(WalkingSound);  // �� �Ҹ� ����
		AudioComponent->Play();  // �� �Ҹ� ���
	}
}

void AMyCharacter::StartWalking()
{
	if (!isWalking)
	{
		isWalking = true;
		bIsMovingForward = true; // W Ű ���� ���·� ����

		// �ȱ� �ִϸ��̼� Ȱ��ȭ
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = true; // �ȱ� �ִϸ��̼� Ȱ��ȭ
		}

		// �ȱ� �Ҹ� ���
		if (WalkingSound && AudioComponent)
		{
			if (AudioComponent->IsPlaying())
			{
				AudioComponent->Stop(); // ���� ��� ���� �Ҹ� ����
			}

			AudioComponent->SetSound(WalkingSound);  // �ȱ� ���� ����
			AudioComponent->Play();  // �ȱ� �Ҹ� ���
			AudioComponent->SetPitchMultiplier(0.74f); // �ӵ��� �´� ��ġ ����
		}
	}
}

// �ȱ� ���� �Լ� ����
void AMyCharacter::StopWalking()
{
	if (isWalking)
	{
		isWalking = false;
		bIsMovingForward = false; // W Ű ���� ���� ����

		// �ȱ� �ִϸ��̼� ��Ȱ��ȭ
		UMyAnimInstance* AnimInst = Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());
		if (AnimInst)
		{
			AnimInst->isBlend = false; // �ȱ� �ִϸ��̼� ��Ȱ��ȭ
		}

		// �ȱ� �Ҹ� ����
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop(); // �ȱ� �Ҹ� ����
		}
	}
}

void AMyCharacter::vertical(float v)
{

	 // W Ű���� �����ϵ��� ����
	if (v > 0.0f)  // W Ű�� ������ ���� �������� �̵�
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



