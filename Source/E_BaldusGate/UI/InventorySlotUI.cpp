// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotUI.h"

#include "InventorySlotEmpty.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
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
	UE_LOG(LogTemp, Warning, TEXT("✅ 모든 마우스 클릭 감지!"));
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 슬롯 아이템 스트럭트 슬롯있음? %i"),ItemStruct.ItemIndex);
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI:: 아이템슬롯 인덱스  %s"), *GetName());
		
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDragDetected"));

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();

	UItemObject* ItemObject = NewObject<UItemObject>();
	ItemObject->ItemStructObject = ItemStruct;
	ItemObject->ItemClientStructObject = ItemClientStruct;

	ItemStruct = FItemStruct();
	ItemClientStruct = FItemClientStruct();

    UInventorySlotEmpty* SlotEmpty = CreateWidget<UInventorySlotEmpty>(GetWorld(),UInventorySlotEmpty::StaticClass());
	if (SlotEmpty)
	{
		// FSlateBrush Brush;
		// Brush.SetResourceObject(ItemObject->ItemClientStructObject.ItemTextures[ItemObject->ItemStructObject.ItemIndex])    ;
		// SlotEmpty->IconImage->SetBrush(Brush);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("인벤슬롯 브러쉬없음"))
	}
	DragOp->DefaultDragVisual = SlotEmpty; // 드래그 시 따라다니는 비주얼 (복사본 만들면 깔끔)
	
	ItemIconImage->SetBrush(FSlateBrush());
	
	DragOp->Payload = ItemObject;
	// UInventorySlotUI* slot = this;
	// UImage* DragImage = NewObject<UImage>(this);
	// DragImage->SetBrush(ItemIconImage->Brush);
	
	DragOp->Pivot = EDragPivot::MouseDown;
	OutOperation = DragOp;
}

bool UInventorySlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDrop"));
	UItemObject* Object = Cast<UItemObject>(InOperation->Payload);
	if (Object)
	{
		Swap(ItemStruct, Object->ItemStructObject);
		Swap(ItemClientStruct, Object->ItemClientStructObject);

		// auto slotUI = Cast<UInventorySlotUI>(InOperation->DefaultDragVisual);
		// slotUI->ItemIconImage->SetBrush(FSlateBrush());
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{ }, 0.5 , false);
		
		FSlateBrush Brush;
		Brush.SetResourceObject(ItemClientStruct.ItemTextures[ItemStruct.ItemIndex]);
		ItemIconImage->SetBrush(Brush);
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI:: 현재 아이템슬롯 인덱스 %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDropNONONONONNONO"));
	}
	// UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::현재 아이템의인덱스 %i"), ItemStruct.ItemIndex);
	// UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::전  아이템의인덱스 %i"), Object->SlotUI->ItemStruct.ItemIndex);

	

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
// UItemObject* ItemObject = Cast<UItemObject>(InOperation->Payload);
// ItemStruct = FItemStruct(ItemObject->ItemStructObject);
// ItemClientStruct = FItemClientStruct(ItemObject->ItemClientStructObject);
	
// UTexture2D* DraggedTexture = ItemClientStruct.ItemTextures[ItemStruct.ItemIndex];
//
// FSlateBrush Brush;
// Brush.SetResourceObject(DraggedTexture);
// ItemIconImage->SetBrush(Brush);
//
// ItemObject->SlotUI->ItemIconImage->SetBrush(FSlateBrush());
// ItemObject->SlotUI->ItemStruct = FItemStruct();
// ItemObject->SlotUI->ItemClientStruct = FItemClientStruct();