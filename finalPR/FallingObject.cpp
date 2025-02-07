#include "FallingObject.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

// Sets default values
AFallingObject::AFallingObject()
{
    // Enable ticking every frame
    PrimaryActorTick.bCanEverTick = true;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결
    
    // 사운드 큐를 불러오기
    static ConstructorHelpers::FObjectFinder<USoundBase> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Engine/EditorSounds/GamePreview/StartPlayInEditor.StartPlayInEditor'"));
    if (HitSoundAsset.Succeeded())
    {
        HitSound = HitSoundAsset.Object;
    }

    // Create and attach components
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetBoxExtent(FVector(400.0f, 400.0f, 600.0f));

    FallingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FallingMesh"));
    FallingMesh->SetupAttachment(RootComponent);

    // Initialize variables
    bHasFallen = false;
    GravityScale = 1.0f;

    // Load the static mesh asset (replace "StaticMesh'/Game/PathToYourMesh.YourMesh'" with your mesh path)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Death_Samurai_Character/death_samurai_character.death_samurai_character'"));
    if (MeshAsset.Succeeded())
    {
        FallingMesh->SetStaticMesh(MeshAsset.Object);
    }
    FallingMesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AFallingObject::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap event
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingObject::OnOverlapBegin);

    FallingMesh->OnComponentHit.AddDynamic(this, &AFallingObject::OnMeshHit);
}

// Called every frame
void AFallingObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Optional: Add custom falling logic if needed
}

void AFallingObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapping actor is the player
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter && !bHasFallen)
    {
        // Enable physics to make the object fall
        FallingMesh->SetSimulatePhysics(true);
        FallingMesh->SetEnableGravity(true);
        FallingMesh->SetPhysicsLinearVelocity(FVector::ZeroVector); // Reset initial velocity

        // Optional: Adjust gravity scale for more control
        FallingMesh->SetMassOverrideInKg(NAME_None, 10.0f, true); // Example mass adjustment

        bHasFallen = true; // Prevent repeated falling
    }
}

void AFallingObject::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Check if the object hit the ground
    if (OtherActor && OtherActor != this)
    {
        // Play sound only if the object hasn't already fallen and hit
        if (bHasFallen && HitSound)
        {
            // Play sound at location using the AudioComponent
            AudioComponent->SetSound(HitSound); // Set the sound
            AudioComponent->Play(); // Play the sound
            bHasFallen = false;  // Reset to prevent multiple sound plays
        }
    }
}
