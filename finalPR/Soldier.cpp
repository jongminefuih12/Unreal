// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Text_UI.h"
#include "MyGameInstance.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 

// Sets default values
ASoldier::ASoldier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����


	static ConstructorHelpers::FObjectFinder<USoundBase> SoldierSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/SoldierSound.SoldierSound'"));
	if (SoldierSoundAsset.Succeeded())
	{
		SoldierSound = SoldierSoundAsset.Object;
	}

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_component"));
	this->RootComponent = capsule;

	capsule->OnComponentBeginOverlap.AddDynamic(this, &ASoldier::player_overlap);

	capsule->SetCapsuleHalfHeight(175.0f); // ĸ���� ���̸� ����
	capsule->SetCapsuleRadius(35.0f);     // ĸ���� �������� ����
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
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

void ASoldier::PlaySoldierSound()
{
	if (AudioComponent && SoldierSound)
	{

		// �� �Ҹ� �����ϰ� ���
		AudioComponent->SetSound(SoldierSound);  // �� �Ҹ� ����
		AudioComponent->Play();  // �� �Ҹ� ���
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AudioComponent or BellSound is not valid"));
	}
}

void ASoldier::ResetAudioComponent()
{
	if (AudioComponent)
	{
		// ����� ������Ʈ�� ���� �ʱ�ȭ
		AudioComponent->SetSound(SoldierSound);  // ���� ������ �� �Ҹ�
		AudioComponent->Play();  // �� �Ҹ� ���
	}
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoldier::ResetText()
{
	// CurrentHour�� �ʱ�ȭ
	CurrentHour = 0;

	// GameInstance���� ����� �� �ʱ�ȭ
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SavedText = FText::FromString(TEXT("00:00"));
		GameInstance->SavedCurrentHour = CurrentHour;
		UE_LOG(LogTemp, Warning, TEXT("Time reset to 00:00 in GameInstance."));
	}

	// �ؽ�Ʈ UI ������Ʈ
	if (TextUIActor)
	{
		TextUIActor->UpdateText(FText::FromString(TEXT("00:00")));
	}
}

void ASoldier::player_overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* sp = Cast<AMyCharacter>(OtherActor);

	if (sp != nullptr)
	{
		// �ؽ�Ʈ UI ������Ʈ
		if (TextUIActor) // TextUIActor�� ��ȿ�� ���
		{

			// �ð��� 1�ð��� ������Ŵ (05�ñ����� ����)
			if (CurrentHour < 6)
			{
				CurrentHour += 1;  // 5�ñ��� ����
			}

			// �ð��� 1�ð��� ������Ŵ
			FString TimeString = FString::Printf(TEXT("%02d:00"), CurrentHour);
			TextUIActor->UpdateText(FText::FromString(TimeString));

			// ���� �ν��Ͻ��� �ؽ�Ʈ ����
			UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());

			if (GameInstance)
			{
				GameInstance->SavedText = FText::FromString(TimeString);
				GameInstance->SavedCurrentHour = CurrentHour; // ���� �ν��Ͻ��� �ð� �� ����
				UE_LOG(LogTemp, Warning, TEXT("Text saved to GameInstance: %s"), *GameInstance->SavedText.ToString());
			}
		}

		sp->GetCharacterMovement()->StopMovementImmediately();
		sp->GetCharacterMovement()->DisableMovement();

		PlaySoldierSound();

		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([this, sp]()
			{
				// �� ��ȯ ó��
				if (CurrentHour == 6)
				{
					// 6�ð� �Ǹ� Ư�� ������ ��ȯ
					FName FinalScene = TEXT("FinalScene"); // ���� �� �̸� ����
					UGameplayStatics::OpenLevel(this, FinalScene);

					ResetText();
				}
				else
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
				}

				ResetAudioComponent();

			});

		// Ÿ�̸ӷ� 4�� �� �� ��ȯ
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 2.0f, false);  // 4�ʷ� ����
		
	}
}
