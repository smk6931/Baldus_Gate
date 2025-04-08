// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
		UE_LOG(LogTemp, Warning, TEXT("✅ 왼쪽 마우스 클릭 감지!"));
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDragDetected"));

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	
	DragOp->Payload = this; // 또는 아이템 정보 구조체
	DragOp->DefaultDragVisual = this; // 드래그 시 따라다니는 비주얼 (복사본 만들면 깔끔)
	// UImage* DragImage = NewObject<UImage>(this);
	// DragImage->SetBrush(ItemIconImage->Brush);
	DragOp->Pivot = EDragPivot::MouseDown;
	OutOperation = DragOp;
}

bool UInventorySlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDrop"));
	UInventorySlotUI* SlotUi = Cast<UInventorySlotUI>(InOperation->Payload);
	
	ItemStruct = FItemStruct(SlotUi->ItemStruct);

	UTexture2D* DraggedTexture = SlotUi->ItemStruct.ItemTextures[SlotUi->ItemStruct.ItemIndex];
	
	FSlateBrush Brush;
	Brush.SetResourceObject(DraggedTexture);
	ItemIconImage->SetBrush(Brush);

	FItemStruct EmptyStruct;
	SlotUi->ItemStruct = EmptyStruct;
	SlotUi->ItemStruct.ItemTextures = ItemStruct.ItemTextures;
	SlotUi->ItemIconImage->SetBrushFromTexture(nullptr);
	SlotUi->ItemIconImage->SetColorAndOpacity(FLinearColor::White);
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
