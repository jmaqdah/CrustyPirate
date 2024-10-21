// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // Note that the RootComponent is already set up for us since we inherit from APaperZDCharacter
    SpringArm->SetupAttachment(RootComponent);
    
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    
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
    
    if (IsAlive && CanMove)
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
    if (IsAlive && CanMove)
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
    
}
