// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotEmpty.h"

void UInventorySlotEmpty::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Visible); // 보장
	this->SetIsEnabled(true); // 활성화
}
