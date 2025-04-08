// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryEquipUI.h"

#include "InventorySlotEmpty.h"
#include "InventorySlotUI.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/VerticalBox.h"
#include "E_BaldusGate/Character/E_BaldusGateCharacter.h"



bool UInventoryEquipUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                     UDragDropOperation* InOperation)
{
	UInventorySlotUI* ItemSlot = Cast<UInventorySlotUI>(InOperation->Payload);
	LeftBox->AddChildToVerticalBox(ItemSlot);

	if (ItemSlot->ItemStruct.ItemTypeIndex == 1)
	{
		AE_BaldusGateCharacter* Character = Cast<AE_BaldusGateCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		Character->AttackWeapon();
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryEquipUI::NativeConstruct()
{
	Super::NativeConstruct();
	for (int32 i = 0; i < 4; i++)
	{ EquipUI = CreateWidget<UInventorySlotUI>(GetWorld(), EquipUIFactory);
		LeftBox->AddChildToVerticalBox(EquipUI); }
	
	for (int32 i = 0; i < 4; i++){
		EquipUI = CreateWidget<UInventorySlotUI>(GetWorld(), EquipUIFactory);
			RightBox->AddChildToVerticalBox(EquipUI); }
}
