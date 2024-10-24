// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
    
public:
    // meta = (BindWidget) allows us to bind the widgets in c++ to the ones in the blueprint
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* HPText;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* DiamondText;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* LevelText;
    
    void SetHP(int NewHP);
    void SetDiamond(int Amount);
    void SetLevel(int Index);
    
};
