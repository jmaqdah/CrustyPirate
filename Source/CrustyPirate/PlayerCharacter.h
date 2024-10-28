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

#include "Engine/TimerHandle.h"

#include "Sound/SoundBase.h"

#include "PlayerHUD.h"
#include "CollectableItem.h"
#include "CrustyPirateGameInstance.h"

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
    UInputAction* QuitAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence* AttackAnimSequence;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerHUD> PlayerHUDClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPlayerHUD* PlayerHUDWidget;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCrustyPirateGameInstance* MyGameInstance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* ItemPickupSound;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsActive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsStunned = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanAttack = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int HitPoints = 100;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int AttackDamage = 25;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackStunDuration = 0.3f;
    
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
    
    FTimerHandle StunTimer;
    FTimerHandle RestartTimer;
    
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
    
    void TakeHit(int DamageAmount, float StunDuration);
    void UpdateHP(int NewHP);
    
    void Stun(float DurationInSeconds);
    
    void OnStunTimerTimeout();
    
    void CollectItem(CollectableType ItemType);
    void UnlockDoubleJump();
	
    void OnRestartTimerTimeout();
    
    void Deactivate();
    
    void QuitGame();
};
