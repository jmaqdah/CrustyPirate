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

#include "PaperZDAnimInstance.h"

#include "Components/BoxComponent.h"

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
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* AttackCollisionBox;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext* InputMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* MoveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* JumpAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* AttackAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence* AttackAnimSequence;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanAttack = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int AttackDamage = 25;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackStunDuration = 0.3f;
    
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
    
    APlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    
    void Move(const FInputActionValue& Value);
    void JumpStarted(const FInputActionValue& Value);
    void JumpEnded(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    
    void UpdateDirection(float MoveDirection);
    
    void OnAttackOverrideAnimEnd(bool Completed);
    
    UFUNCTION()
    void AttackBoxOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION(BlueprintCallable)
    void EnableAttackCollisionBox(bool Enabled);
	
};
