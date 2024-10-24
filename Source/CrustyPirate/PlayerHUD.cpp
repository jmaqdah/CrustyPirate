// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetHP(int NewHP)
{
    FString str = FString::Printf(TEXT("HP: %d"), NewHP);
    HPText->SetText(FText::FromString(str));
}

void UPlayerHUD::SetDiamond(int Amount)
{
    FString str = FString::Printf(TEXT("Diamonds: %d"), Amount);
    DiamondText->SetText(FText::FromString(str));
}

void UPlayerHUD::SetLevel(int Index)
{
    FString str = FString::Printf(TEXT("Level: %d"), Index);
    LevelText->SetText(FText::FromString(str));
}
