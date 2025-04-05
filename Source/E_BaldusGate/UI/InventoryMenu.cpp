// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (WBP_Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryMenu::NativeConstruct"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryMenu::NONONONON"));
	}
}
