// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()
    
public:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCameraComponent* Camera;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext* InputMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* MoveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* JumpAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* AttackAction;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanMove = true;
    
    APlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    
    void Move(const FInputActionValue& Value);
    void JumpStarted(const FInputActionValue& Value);
    void JumpEnded(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    
    void UpdateDirection(float MoveDirection);
	
};
