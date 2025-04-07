// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UInventorySlotUI::NativeConstruct()
{
	Super::NativeConstruct();
    // SlotItemBt->OnClicked.AddDynamic(this,&UInventorySlotUI::OnCliced);
	
	this->SetVisibility(ESlateVisibility::Visible); // 보장
	this->SetIsEnabled(true); // 활성화
}

// void UInventorySlotUI::OnCliced()
// {
// 	UE_LOG(LogTemp, Display, TEXT("InventorySlotUI::OnCliced"));
// }

FReply UInventorySlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ 왼쪽 마우스 클릭 감지!"));
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ 오른쪽 마우스 클릭 감지!"));
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;

	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::LeftMouseButton"));
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlotUI::NativeOnDragDetected"));

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->Payload = this; // 또는 아이템 정보 구조체
	DragOp->DefaultDragVisual = this; // 드래그 시 따라다니는 비주얼 (복사본 만들면 깔끔)
	DragOp->Pivot = EDragPivot::MouseDown;

	OutOperation = DragOp;
}