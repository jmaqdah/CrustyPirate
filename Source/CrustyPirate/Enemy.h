// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

#include "Components/BoxComponent.h"

#include "PaperZDAnimInstance.h"

#include "Engine/TimerHandle.h"

#include "PlayerCharacter.h"

#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* PlayerDetectorSphere;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UTextRenderComponent* HPText;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* AttackCollisionBox;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerCharacter* FollowTarget;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence* AttackAnimSequence;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StopDistanceToTarget = 70.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int HitPoints = 100;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackCoolDownInSeconds = 3.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int AttackDamage = 25;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackStunDuration = 0.3f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsStunned = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanAttack = true;
    
    FTimerHandle StunTimer;
    
    FTimerHandle AttackCoolDownTimer;
    
    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
    
    AEnemy();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION()
    void DetectorOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void DetectorOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
    bool ShouldMoveToTarget();
    void UpdateDirection(float MoveDirection);
    
    void UpdateHP(int NewHP);
    
    void TakeHit(int DamageAmount, float StunDuration);
    
    void Stun(float DurationInSeconds);
    void OnStunTimerTimeout();
    
    void Attack();
    void OnAttackCoolDownTimerTimeout();
    void OnAttackOverrideAnimEnd(bool Completed);
    
    UFUNCTION()
    void AttackBoxOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION(BlueprintCallable)
    void EnableAttackCollisionBox(bool Enabled);
    
};
