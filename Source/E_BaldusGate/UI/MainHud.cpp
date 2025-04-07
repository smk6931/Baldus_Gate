// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHud.h"

#include "InventorySlotUI.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"

bool UMainHud::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                            UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Display, TEXT("UMainHud::NativeOnDrop"));
	UInventorySlotUI* HudSlot = Cast<UInventorySlotUI>(InOperation->Payload);
	if (HudSlot)
	{
		UE_LOG(LogTemp, Display, TEXT("UMainHud:아이템있음"));
		FSlateBrush brush;
		brush.SetResourceObject(HudSlot->ItemStruct.ItemTextures[HudSlot->ItemStruct.ItemIndex]);
		HudImage->SetBrush(brush);
		ItemStruct = HudSlot->ItemStruct;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("UMainHud:아이템없음"));
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
