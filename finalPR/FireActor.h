// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireActor.generated.h"

UCLASS()
class FINALPR_API AFireActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StopFireSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FireSound;

	void PlayFireSound();

private:
    UPROPERTY(EditAnywhere, Category = "Fire Settings")
    float MaxScale = 10.0f; // 최대 크기

    UPROPERTY(EditAnywhere, Category = "Fire Settings")
    float MinScale = 0.5f; // 최소 크기

    UPROPERTY(EditAnywhere, Category = "Fire Settings")
    float EffectRadius = 1000.0f; // 효과 범위 (캐릭터와 불의 거리)

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* FireMesh; // 불의 시각적 표현

    ACharacter* PlayerCharacter;

    void UpdateFireScale(float Distance);
};
