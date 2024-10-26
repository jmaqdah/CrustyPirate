// Fill out your copyright notice in the Description page of Project Settings.


#include "CrustyPirateGameInstance.h"

void UCrustyPirateGameInstance::SetPlayerHP(int NewHP)
{
    PlayerHP = NewHP;
}


void UCrustyPirateGameInstance::AddDiamond(int Amount)
{
    CollectedDiamondCount += Amount;
}
