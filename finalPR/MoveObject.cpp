// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveObject.h"

// Sets default values
AMoveObject::AMoveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	smc = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMC"));
	smc->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		obj(TEXT("/Script/Engine.SkeletalMesh'/Game/KTInteractionPropVol1/Mesh/Noglo/SK_Noglo.SK_Noglo'"));

	if (obj.Succeeded())
	{
		smc->SetSkeletalMesh(obj.Object);
		smc->SetRelativeLocation(FVector(0, 0, 0));
	}

}

// Called when the game starts or when spawned
void AMoveObject::BeginPlay()
{
	Super::BeginPlay();
	
	

}

// Called every frame
void AMoveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	playerTime += DeltaTime * 3.0f;


	FVector pos = GetActorLocation();
	FRotator rot = GetActorRotation();

	pos.Z += FMath::Sin(playerTime) * 1.0f;

	rot.Yaw += FMath::Sin(playerTime) * 15.0f;

	SetActorLocationAndRotation(pos, rot);

}


