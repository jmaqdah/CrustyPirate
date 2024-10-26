// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrustyPirateGameInstance.generated.h"

/**
 * The game instance lives for the whole of the game. The player is destroyed between levels.
 * We can use the game instance to retain information about the player between levels (such as HP)
 */
UCLASS()
class CRUSTYPIRATE_API UCrustyPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int PlayerHP = 100;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int CollectedDiamondCount = 0;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDoubleJumpUnlocked = false;
    
    void SetPlayerHP(int NewHP);
    void AddDiamond(int Amount);
    
	
};