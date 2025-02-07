// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"  // UBoxComponent ��� �߰�
#include "Components/StaticMeshComponent.h"  // UStaticMeshComponent ��� �߰�
#include "FallingObject.generated.h"

UCLASS()
class FINALPR_API AFallingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    UAudioComponent* AudioComponent;  // ����� ������Ʈ ����

private:
    // Trigger box to detect player proximity
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

    // Mesh that will fall
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* FallingMesh;

    // Whether the object has already fallen
    bool bHasFallen;

    // Gravity scale for the object
    float GravityScale;

    UPROPERTY(EditAnywhere)
    USoundBase* HitSound;

    // Overlap event function
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
