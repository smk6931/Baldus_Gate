// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "InventorySlotUI.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/VerticalBox.h"
#include "E_BaldusGate/Character/E_BaldusGateCharacter.h"

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UInventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Display, TEXT("InventoryMenu::NativeOnDrop"));
	
	UInventorySlotUI* ItemSlot = Cast<UInventorySlotUI>(InOperation->Payload);
	
	LeftBox->AddChildToVerticalBox(ItemSlot);
	if (ItemSlot->ItemStruct.ItemTypeIndex == 1) // 1번은 웨폰
	{
		UE_LOG(LogTemp, Display, TEXT("InventoryMenu::NativeOnDrop: ItemTypeIndex 1 무기임"));
		AE_BaldusGateCharacter* Character = Cast<AE_BaldusGateCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (Character)
		{
			UE_LOG(LogTemp, Display, TEXT("InventoryMenu::NativeOnDrop: 캐릭터있다"));
			Character->AttackWeapon();
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
