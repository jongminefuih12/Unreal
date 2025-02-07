// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalBell2.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Text_UI.h"
#include "MyGameInstance.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 

float Direction = 1.0f;

// Sets default values
AAbnormalBell2::AAbnormalBell2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����


	static ConstructorHelpers::FObjectFinder<USoundBase> BellSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/BellSound.BellSound'"));
	if (BellSoundAsset.Succeeded())
	{
		BellSound = BellSoundAsset.Object;
	}

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_component"));
	this->RootComponent = capsule;

	capsule->OnComponentBeginOverlap.AddDynamic(this, &AAbnormalBell2::player_overlap);

	capsule->SetCapsuleHalfHeight(100.0f); // ĸ���� ���̸� ����
	capsule->SetCapsuleRadius(170.0f);     // ĸ���� �������� ����
}

// Called when the game starts or when spawned
void AAbnormalBell2::BeginPlay()
{
	Super::BeginPlay();

	// �������� ��� AText_UI ���͸� ã��
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AText_UI::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		// �������� ã�� ù ��° AText_UI ���͸� TextUIActor�� �Ҵ�
		TextUIActor = Cast<AText_UI>(FoundActors[0]);
		if (TextUIActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("TextUIActor found and connected."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TextUIActor not found in level!"));
	}

	// ���� �ν��Ͻ����� ����� �ð� �ε�
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		CurrentHour = GameInstance->SavedCurrentHour; // ����� �ð� �� �ε�
	}

	// �� �̵� �� �ؽ�Ʈ ������ ���� �ؽ�Ʈ �ʱ� ������Ʈ
	if (TextUIActor)
	{
		FString InitialTime = FString::Printf(TEXT("%02d:00"), CurrentHour);
		TextUIActor->UpdateText(FText::FromString(InitialTime));
	}
}

void AAbnormalBell2::PlayBellSound()
{
	if (AudioComponent && BellSound)
	{
		// ������ ��� ���� �Ҹ� ����
	   // AudioComponent->Stop();

		// �� �Ҹ� �����ϰ� ���
		AudioComponent->SetSound(BellSound);  // �� �Ҹ� ����
		AudioComponent->Play();  // �� �Ҹ� ���
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioComponent or BellSound is not valid"));
	}
}

void AAbnormalBell2::ResetAudioComponent()
{
	if (AudioComponent)
	{
		// ����� ������Ʈ�� ���� �ʱ�ȭ
		//AudioComponent->Stop();  // �Ҹ� ����
		AudioComponent->SetSound(BellSound);  // ���� ������ �� �Ҹ�
		AudioComponent->Play();  // �� �Ҹ� ���
	}
}

// Called every frame
void AAbnormalBell2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	playerTime += DeltaTime * 4.0f;

	FVector pos = GetActorLocation();
	FRotator rot = GetActorRotation();

	pos.Y += Direction * 500.0f * DeltaTime;

	// ��ġ �� ȸ�� ����
	SetActorLocationAndRotation(pos, rot);

	// X ��ġ�� -2700 �̻��� �� ���� ��ȯ
	if (pos.Y >= -170.0f)
	{
		Direction = -1.0f; // �������� �̵�
	}
	// X ��ġ�� -2780 ������ �� ���� ��ȯ
	else if (pos.Y <= -2780.0f)
	{
		Direction = 1.0f; // ���������� �̵�
	}
}

void AAbnormalBell2::player_overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* sp = Cast<AMyCharacter>(OtherActor);

	//if (sp != nullptr)
	//{
	//	// ���ο� �� �̸� (Level �̸��� �𸮾� �������� �� �̸��� �����ϰ� ����)
	//	FName NextLevelName = TEXT("SecondScene");

	//	// �� ��ȯ ����
	//	UGameplayStatics::OpenLevel(this, NextLevelName);
	//}

	if (sp != nullptr)
	{
		// �ؽ�Ʈ UI ������Ʈ
		if (TextUIActor) // TextUIActor�� ��ȿ�� ���
		{
			// �ð��� 00:00���� �����Ͽ� ������Ʈ
			FString TimeString = TEXT("00:00");
			TextUIActor->UpdateText(FText::FromString(TimeString));

			// ���� �ν��Ͻ��� �ؽ�Ʈ ����
			UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
			if (GameInstance)
			{
				GameInstance->SavedText = FText::FromString(TimeString);
				GameInstance->SavedCurrentHour = 0; // �ð� ���� 0���� �ʱ�ȭ
				UE_LOG(LogTemp, Warning, TEXT("Text saved to GameInstance: %s"), *GameInstance->SavedText.ToString());
			}
		}
		sp->GetCharacterMovement()->StopMovementImmediately();
		sp->GetCharacterMovement()->DisableMovement();

		PlayBellSound();


		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([this, sp]()
			{
				TArray<FName> LevelNames = {
					TEXT("ThirdScene"), TEXT("ForthScene"),
					TEXT("FifthScene"), TEXT("SixthScene"), TEXT("SeventhScene"), TEXT("EigthScene"),
					TEXT("NinthScene"), TEXT("TenthScene"), TEXT("EleventhScene"), TEXT("12thScene"),
					TEXT("13thScene"), TEXT("14thScene"), TEXT("15thScene"), TEXT("16Scene"),
					TEXT("17Scene"), TEXT("18Scene"), TEXT("19Scene"), TEXT("20Scene"),
					TEXT("21Scene"), TEXT("22Scene"), TEXT("23Scene"), TEXT("24Scene"),
					TEXT("25Scene"), TEXT("26Scene"), TEXT("27Scene"), TEXT("28Scene"),
					TEXT("29Scene"), TEXT("30Scene"), TEXT("31Scene"), TEXT("32Scene"),
					TEXT("33Scene"), TEXT("34Scene"), TEXT("35Scene"), TEXT("36Scene"),
					TEXT("37Scene"), TEXT("38Scene"), TEXT("39Scene"), TEXT("40Scene"),
				};

				// �������� �ϳ��� ���� ����
				int32 RandomIndex = FMath::RandRange(0, LevelNames.Num() - 1);
				FName NextLevelName = LevelNames[RandomIndex];

				// ���õ� ������ ��ȯ
				UGameplayStatics::OpenLevel(this, NextLevelName);

				// �� ��ȯ �� ����� ����
				ResetAudioComponent();
			});

		// Ÿ�̸ӷ� 3.5�� �� �� ��ȯ
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 3.0f, false); // 3.5�ʷ� ����
		sp->GetCharacterMovement()->StopMovementImmediately();
	}
}