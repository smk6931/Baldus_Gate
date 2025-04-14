// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotUI.h"

#include "InventorySlotEmpty.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "E_BaldusGate/Character/E_BaldusGateCharacter.h"
#include "E_BaldusGate/Item/Item.h"
#include "E_BaldusGate/Item/ItemObject.h"

void UInventorySlotUI::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Visible); // 보장
	this->SetIsEnabled(true); // 활성화
}

FReply UInventorySlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// UE_LOG(LogTemp, Warning, TEXT("인벤토리 슬롯 아이템 스트럭트 슬롯있음? %i"),ItemStruct.ItemIndex);
		// UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI:: 아이템슬롯 인덱스  %s"), *GetName());
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();

	UItemObject* ItemObject = NewObject<UItemObject>();
	
	ItemObject->ItemStructObject = ItemStruct;
	ItemObject->ItemClientStructObject = ItemClientStruct;

	ItemStruct = FItemStruct();
	ItemClientStruct = FItemClientStruct();

    UInventorySlotEmpty* SlotEmpty = CreateWidget<UInventorySlotEmpty>(GetWorld(),SlotEmptyFactory);
	if (SlotEmpty)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(ItemObject->ItemClientStructObject.ItemTextures[ItemObject->ItemStructObject.ItemIndex])    ;
		SlotEmpty->IconImage->SetBrush(Brush);
	}
	DragOp->DefaultDragVisual = SlotEmpty; // 드래그 시 따라다니는 비주얼 (복사본 만들면 깔끔)
	
	ItemIconImage->SetBrush(FSlateBrush());
	
	DragOp->Payload = ItemObject;
	
	DragOp->Pivot = EDragPivot::MouseDown;
	OutOperation = DragOp;
}

bool UInventorySlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UItemObject* Object = Cast<UItemObject>(InOperation->Payload);
	if (Object)
	{
		Swap(ItemStruct, Object->ItemStructObject);
		Swap(ItemClientStruct, Object->ItemClientStructObject);
		
		FSlateBrush Brush;
		Brush.SetResourceObject(ItemClientStruct.ItemTextures[ItemStruct.ItemIndex]);
		ItemIconImage->SetBrush(Brush);
		
		EquipSlot();
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlotUI::EquipSlot()
{
	if (SlotType == ESlotType::EquipType && ItemStruct.ItemTypeIndex == 1)
	{
		AE_BaldusGateCharacter* Player = Cast<AE_BaldusGateCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		// ItemStruct.ItemIndex = 1;
		// ItemStruct.ItemTypeIndex = 1;
		
		Player->AttachWeapon(ItemStruct);
		UE_LOG(LogTemp,Warning,TEXT("슬롯 장비 무기 장착"))
	}
}
