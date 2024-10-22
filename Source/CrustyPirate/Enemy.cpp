// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"


AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    
    PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
    // Note that the RootComponent is already set up for us since we inherit from APaperZDCharacter
    PlayerDetectorSphere->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind the delegates to the Sphere's events
    PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
    PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // If the enemy is alive and has a follow target assigned, follow the player
    if (IsAlive && FollowTarget)
    {
        // Get the direction of the enemy relative to the player
        float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
        if (CanMove)
        {
            // Move towards the player
            FVector WorldDirection = FVector(1.0f, 0.0f, 0.0f);
            AddMovementInput(WorldDirection, MoveDirection);
        }
        
    }
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    
    // If the casting worked then we know that the player is the actor that entered the sphere
    if (Player)
    {
        FollowTarget = Player;
    }
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    
    // If the casting worked then we know that the player is the actor that exited the sphere
    if (Player)
    {
        FollowTarget = NULL;
    }
}

