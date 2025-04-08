// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "Components/WrapBox.h"

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int32 i = 0 ; i < 24; i++)
	{
		SlotUi = CreateWidget<UInventorySlotUI>(GetWorld(),SlotUIFactory);
		BoxSlot->AddChild(SlotUi);
		UE_LOG(LogTemp,Warning,TEXT("%i"),i)
	}
}
