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
    float MaxScale = 10.0f; // �ִ� ũ��

    UPROPERTY(EditAnywhere, Category = "Fire Settings")
    float MinScale = 0.5f; // �ּ� ũ��

    UPROPERTY(EditAnywhere, Category = "Fire Settings")
    float EffectRadius = 1000.0f; // ȿ�� ���� (ĳ���Ϳ� ���� �Ÿ�)

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* FireMesh; // ���� �ð��� ǥ��

    ACharacter* PlayerCharacter;

    void UpdateFireScale(float Distance);
};
