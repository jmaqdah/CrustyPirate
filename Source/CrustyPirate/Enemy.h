// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Components/SphereComponent.h"

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
    APlayerCharacter* FollowTarget;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StopDistanceToTarget = 70.0f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsAlive = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanMove = true;
    
    AEnemy();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION()
    void DetectorOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void DetectorOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
};
