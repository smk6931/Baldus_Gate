// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHud.h"

bool UMainHud::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Display, TEXT("UMainHud::NativeOnDrop"));
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
