// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // Note that the RootComponent is already set up for us since we inherit from APaperZDCharacter
    SpringArm->SetupAttachment(RootComponent);
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    
    AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
    AttackCollisionBox->SetupAttachment(RootComponent);
    
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Add Input Mapping Context to the player
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }
    
    // Binding the attack animation end delegate (signal) to OnAttackOverrideAnimEnd()
    OnAttackOverrideEndDelegate.BindUObject(this, &APlayerCharacter::OnAttackOverrideAnimEnd);
    
    // Binding the collision box's OnComponentBeginOverlap event to AttackBoxOverlapBegin()
    AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AttackBoxOverlapBegin);
    
    // Disbale the collision box at first
    EnableAttackCollisionBox(false);
    
    // Create the HUD Widget
    if (PlayerHUDClass)
    {
        // Create a widget of class PlayerHUDClass
        PlayerHUDWidget = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerHUDClass);
        
        if (PlayerHUDWidget)
        {
            // Add the widget to the game
            PlayerHUDWidget->AddToPlayerScreen();
            
            // Set the widget texts
            PlayerHUDWidget->SetHP(HitPoints);
            PlayerHUDWidget->SetDiamond(50);
            PlayerHUDWidget->SetLevel(1);
            
        }
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Set up input action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStarted);
        
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnded);
        
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnded);
        
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
        
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    // Direction of player ("D" key --> +1, "S" key --> -1)
    float MoveActionValue = Value.Get<float>();
    
    if (IsAlive && CanMove && !IsStunned)
    {
        FVector Direction = FVector(1.0f, 0.0f, 0.0f);
        AddMovementInput(Direction, MoveActionValue);
        
        // Update player direction
        UpdateDirection(MoveActionValue);
    }
}

void APlayerCharacter::UpdateDirection(float MoveDirection)
{
    // Get the Actor's current rotation
    FRotator CurrentRotation = Controller->GetControlRotation();
    
    // Rotate the Actor
    if (MoveDirection < 0.0f) // Going to the left
    {
        // If not already turned left, turn left
        if (CurrentRotation.Yaw != 180.0f)
        {
            Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
        }
    }
    else if (MoveDirection > 0.0f) // Going to the right
    {
        // If not already turned right, turn right
        if (CurrentRotation.Yaw != 0.0f)
        {
            Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
        }
    }
}

void APlayerCharacter::JumpStarted(const FInputActionValue& Value)
{
    if (IsAlive && CanMove && !IsStunned)
    {
        Jump();
    }
}

void APlayerCharacter::JumpEnded(const FInputActionValue& Value)
{
    StopJumping();
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    if (IsAlive && CanAttack && !IsStunned)
    {
        CanAttack = false;
        CanMove = false;
        
        // Enable the collision box
        //EnableAttackCollisionBox(true);
        
        // Override the current animation sequence with AttackAnimSequence when the player is attacking
        // Once the animation is over, the OnAttackOverrideEndDelegate will be actioned and OnAttackOverrideAnimEnd will be called
        GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
    }
}

void APlayerCharacter::OnAttackOverrideAnimEnd(bool Completed)
{
    CanAttack = true;
    CanMove = true;
    
    // Disable the collision box
    //EnableAttackCollisionBox(false);
}

void APlayerCharacter::AttackBoxOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the actor in the collision box is an enemy actor
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    
    if (Enemy)
    {
        Enemy->TakeHit(AttackDamage, AttackStunDuration);
    }
    
    
}

void APlayerCharacter::EnableAttackCollisionBox(bool Enabled)
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

void APlayerCharacter::TakeHit(int DamageAmount, float StunDuration)
{
    if (!IsAlive) return;
    
    Stun(StunDuration);
    
    UpdateHP(HitPoints - DamageAmount);
    
    if (HitPoints <= 0)
    {
        // Player is dead
        UpdateHP(0);
        
        IsAlive = false;
        CanMove = false;
        CanAttack = false;
        
        // Play the player dead animation
        GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CaptainStateMachine"));
        
        // Disable the attack collision box
        EnableAttackCollisionBox(false);
    }
    else
    {
        // Player is still alive
        // Play the player take hit animation
        GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CaptainStateMachine"));
    }
    
    
}

void APlayerCharacter::UpdateHP(int NewHP)
{
    HitPoints = NewHP;
    
    // Update the HUD Widget HP text
    PlayerHUDWidget->SetHP(HitPoints);
}

void APlayerCharacter::Stun(float DurationInSeconds)
{
    IsStunned = true;
    
    // Allow the player to stun the enemy several times
    bool IsTimerAlreadyActive = GetWorldTimerManager().IsTimerActive(StunTimer);
    if (IsTimerAlreadyActive)
    {
        GetWorldTimerManager().ClearTimer(StunTimer);
    }
    
    GetWorldTimerManager().SetTimer(StunTimer, this, &APlayerCharacter::OnStunTimerTimeout, 1.0f, false, DurationInSeconds);
    
    // Make sure we stop any currently playing override animations while stunned (such as attack)
    GetAnimInstance()->StopAllAnimationOverrides();
    
    // Disable the collision box
    EnableAttackCollisionBox(false);
}

void APlayerCharacter::OnStunTimerTimeout()
{
    IsStunned = false;
    
}
