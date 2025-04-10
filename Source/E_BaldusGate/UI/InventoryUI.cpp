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
		// UE_LOG(LogTemp,Warning,TEXT("%i"),i)
	}
}

void UInventoryUI::PrintSlots()
{
	FString text = "";
	int i = 0;
	for (auto slot : BoxSlot->GetAllChildren())
	{
		auto ui = Cast<UInventorySlotUI>(slot);
		if (ui)
		{
			text += ui->ItemStruct.ItemIndex + TEXT(", ");
		}

		i++;
		if (i % 4 == 0)
		{
			text += TEXT("\n");
		}
	}

	UE_LOG(LogTemp, Display, TEXT("%s"), *text);
}
