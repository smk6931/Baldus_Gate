// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotEmpty.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UInventorySlotEmpty : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Count;
};
