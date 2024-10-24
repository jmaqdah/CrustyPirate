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
    
    AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
    AttackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind the delegates to the Sphere's events
    PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
    PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
    
    UpdateHP(HitPoints);
    
    // Binding the attack animation end delegate (signal) to OnAttackOverrideAnimEnd()
    OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);
    
    // Binding the collision box's OnComponentBeginOverlap event to AttackBoxOverlapBegin()
    AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
    
    // Disable the collision box at first
    EnableAttackCollisionBox(false);
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
            if (FollowTarget->IsAlive)
            {
                Attack();
            }
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
    
    // Stun the enemy (This makes sure the override animations (such as the attack one) is stopped in case
    // the enemy was attacking while the player attacks)
    Stun(StunDuration);
    
    UpdateHP(HitPoints - DamageAmount);
    
    if (HitPoints <= 0)
    {
        // Enemy is dead
        UpdateHP(0);
        HPText->SetHiddenInGame(true);
        IsAlive = false;
        CanMove = false;
        CanAttack = false;
        
        // Play the die animation by jumpting to the JumpDie animation
        GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CrabbyStateMachine"));
        
        // Disable the collision box after the enemy is dead
        EnableAttackCollisionBox(false);
        
    }
    else
    {
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
    
    // Make sure we stop any currently playing override animations while stunned (such as attack)
    GetAnimInstance()->StopAllAnimationOverrides();
    
    // Disable the collision box
    EnableAttackCollisionBox(false);
}

void AEnemy::OnStunTimerTimeout()
{
    IsStunned = false;
    
}

void AEnemy::Attack()
{
    if (IsAlive && CanAttack && !IsStunned)
    {
        CanAttack = false;
        CanMove = false;
        
        // Override the current animation sequence with AttackAnimSequence when the enemy is attacking
        // Once the animation is over, the OnAttackOverrideEndDelegate will be actioned and OnAttackOverrideAnimEnd will be called
        // We allow the enemy to move when the attack animation ends
        GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
        
        // We dont want the enemy to attack as soon as hes done attacking, we want the enemy to attack after the cool down
        GetWorldTimerManager().SetTimer(AttackCoolDownTimer, this, &AEnemy::OnAttackCoolDownTimerTimeout, 1.0f, false, AttackCoolDownInSeconds);
    }
}

void AEnemy::OnAttackCoolDownTimerTimeout()
{
   if (IsAlive)
   {
       CanAttack = true;
   }
}

void AEnemy::OnAttackOverrideAnimEnd(bool Completed)
{
    if (IsAlive)
    {
        CanMove = true;
    }
}

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the object entering the collision box is the player
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    
    if (Player)
    {
        Player->TakeHit(AttackDamage, AttackStunDuration);
    }
}

void AEnemy::EnableAttackCollisionBox(bool Enabled)
{
    if (Enabled)
    {
        // Enable the collision box
        AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        // Setting the collision response to the pawn to be overalp
        AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    }
    else
    {
        // Disable the collision box
        AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        // Setting the collision response to the pawn to be ignore
        AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    }
}
