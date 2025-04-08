// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotUI.h"
#include "Blueprint/UserWidget.h"
#include "InventoryEquipUI.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UInventoryEquipUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* LeftBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* RightBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotUI>EquipUIFactory;

	UPROPERTY(EditAnywhere)
	class UInventorySlotUI* EquipUI;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeConstruct() override;
};
