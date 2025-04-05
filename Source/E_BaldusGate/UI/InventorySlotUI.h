// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "E_BaldusGate/Item/ItemStruct.h"
#include "InventorySlotUI.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UInventorySlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ItemIconImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ItemCount;
	
};
