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
	AudioComponent->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결


	static ConstructorHelpers::FObjectFinder<USoundBase> SoldierSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/SoldierSound.SoldierSound'"));
	if (SoldierSoundAsset.Succeeded())
	{
		SoldierSound = SoldierSoundAsset.Object;
	}

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule_component"));
	this->RootComponent = capsule;

	capsule->OnComponentBeginOverlap.AddDynamic(this, &ASoldier::player_overlap);

	capsule->SetCapsuleHalfHeight(175.0f); // 캡슐의 높이를 설정
	capsule->SetCapsuleRadius(35.0f);     // 캡슐의 반지름을 설정
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{
	Super::BeginPlay();

	// 레벨에서 모든 AText_UI 액터를 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AText_UI::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		// 레벨에서 찾은 첫 번째 AText_UI 액터를 TextUIActor에 할당
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

	// 게임 인스턴스에서 저장된 시간 로드
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		CurrentHour = GameInstance->SavedCurrentHour; // 저장된 시간 값 로드
	}

	// 맵 이동 시 텍스트 갱신을 위해 텍스트 초기 업데이트
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

		// 종 소리 설정하고 재생
		AudioComponent->SetSound(SoldierSound);  // 종 소리 설정
		AudioComponent->Play();  // 종 소리 재생
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
		// 오디오 컴포넌트를 새로 초기화
		AudioComponent->SetSound(SoldierSound);  // 새로 설정된 종 소리
		AudioComponent->Play();  // 종 소리 재생
	}
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoldier::ResetText()
{
	// CurrentHour를 초기화
	CurrentHour = 0;

	// GameInstance에서 저장된 값 초기화
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SavedText = FText::FromString(TEXT("00:00"));
		GameInstance->SavedCurrentHour = CurrentHour;
		UE_LOG(LogTemp, Warning, TEXT("Time reset to 00:00 in GameInstance."));
	}

	// 텍스트 UI 업데이트
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
		// 텍스트 UI 업데이트
		if (TextUIActor) // TextUIActor가 유효한 경우
		{

			// 시간을 1시간씩 증가시킴 (05시까지만 증가)
			if (CurrentHour < 6)
			{
				CurrentHour += 1;  // 5시까지 증가
			}

			// 시간을 1시간씩 증가시킴
			FString TimeString = FString::Printf(TEXT("%02d:00"), CurrentHour);
			TextUIActor->UpdateText(FText::FromString(TimeString));

			// 게임 인스턴스에 텍스트 저장
			UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());

			if (GameInstance)
			{
				GameInstance->SavedText = FText::FromString(TimeString);
				GameInstance->SavedCurrentHour = CurrentHour; // 게임 인스턴스에 시간 값 저장
				UE_LOG(LogTemp, Warning, TEXT("Text saved to GameInstance: %s"), *GameInstance->SavedText.ToString());
			}
		}

		sp->GetCharacterMovement()->StopMovementImmediately();
		sp->GetCharacterMovement()->DisableMovement();

		PlaySoldierSound();

		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([this, sp]()
			{
				// 맵 전환 처리
				if (CurrentHour == 6)
				{
					// 6시가 되면 특정 맵으로 전환
					FName FinalScene = TEXT("FinalScene"); // 최종 맵 이름 설정
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

					// 랜덤으로 하나의 맵을 선택
					int32 RandomIndex = FMath::RandRange(0, LevelNames.Num() - 1);
					FName NextLevelName = LevelNames[RandomIndex];

					// 선택된 맵으로 전환
					UGameplayStatics::OpenLevel(this, NextLevelName);
				}

				ResetAudioComponent();

			});

		// 타이머로 4초 후 맵 전환
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 2.0f, false);  // 4초로 설정
		
	}
}
