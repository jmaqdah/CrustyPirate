// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"


AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    
    PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
    // Note that the RootComponent is already set up for us since we inherit from APaperZDCharacter
    PlayerDetectorSphere->SetupAttachment(RootComponent);
    
    HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
    HPText->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind the delegates to the Sphere's events
    PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
    PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
    
    UpdateHP(HitPoints);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // If the enemy is alive and has a follow target assigned, follow the player
    if (IsAlive && FollowTarget && !IsStunned)
    {
        // Get the direction of the enemy relative to the player
        float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
        // Update the enemy direction
        UpdateDirection(MoveDirection);
        // Move to target if not close enough
        if (ShouldMoveToTarget())
        {
            if (CanMove)
            {
                // Move towards the player
                FVector WorldDirection = FVector(1.0f, 0.0f, 0.0f);
                AddMovementInput(WorldDirection, MoveDirection);
            }
        }
        // Otherwise we are close enough and should attack
        else
        {
            // Attack
        }
    }
}

void AEnemy::UpdateDirection(float MoveDirection)
{
    // Get the Actor's current rotation
    FRotator CurrentRotation = GetActorRotation();
    
    // Rotate the Actor
    if (MoveDirection < 0.0f) // Going to the left
    {
        // If not already turned left, turn left
        if (CurrentRotation.Yaw != 180.0f)
        {
            SetActorRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
        }
    }
    else if (MoveDirection > 0.0f) // Going to the right
    {
        // If not already turned right, turn right
        if (CurrentRotation.Yaw != 0.0f)
        {
            SetActorRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
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


bool AEnemy::ShouldMoveToTarget()
{
    bool Result = false;
    
    if (FollowTarget)
    {
        // Calculate distance from the enemy to the target
        float DistanceToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
        
        Result = DistanceToTarget > StopDistanceToTarget;
    }
    
    return Result;
}

void AEnemy::UpdateHP(int NewHP)
{
    // Update Hit Points
    HitPoints = NewHP;
    // Update the Hit Points string (displayed above the enemy)
    FString Str = FString::Printf(TEXT("HP: %d"), HitPoints);
    HPText->SetText(FText::FromString(Str));
}

void AEnemy::TakeHit(int DamageAmount, float StunDuration)
{
    if (!IsAlive) return;
    
    UpdateHP(HitPoints - DamageAmount);
    
    if (HitPoints <= 0)
    {
        // Enemy is dead
        UpdateHP(0);
        HPText->SetHiddenInGame(true);
        IsAlive = false;
        CanMove = false;
        
        // Play the die animation by jumpting to the JumpDie animation
        GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CrabbyStateMachine"));
    }
    else
    {
        // Stun the enemy
        Stun(StunDuration);
        
        // Play the takehit animation by jumpting to the JumpTakeHit animation
        GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CrabbyStateMachine"));
    }
}

void AEnemy::Stun(float DurationInSeconds)
{
    IsStunned = true;
    
    // Allow the player to stun the enemy several times
    bool IsTimerAlreadyActive = GetWorldTimerManager().IsTimerActive(StunTimer);
    if (IsTimerAlreadyActive)
    {
        GetWorldTimerManager().ClearTimer(StunTimer);
    }
    
    GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemy::OnStunTimerTimeout, 1.0f, false, DurationInSeconds);
    
    // Make sure we don't switch to any other animation while stunned
    GetAnimInstance()->StopAllAnimationOverrides();
}

void AEnemy::OnStunTimerTimeout()
{
    IsStunned = false;
    
}
