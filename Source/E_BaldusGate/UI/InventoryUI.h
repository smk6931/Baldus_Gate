// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotUI.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWrapBox* BoxSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotUI> SlotUIFactory;
	
	UPROPERTY(EditAnywhere)
	class UInventorySlotUI* SlotUi;
	
	virtual void NativeConstruct() override;
};
