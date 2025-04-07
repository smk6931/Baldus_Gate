// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "E_BaldusGate/Item/ItemStruct.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UMainHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
	FItemStruct ItemStruct;
	
	UPROPERTY(Meta = (BindWidget))
	class UImage* HudImage;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
