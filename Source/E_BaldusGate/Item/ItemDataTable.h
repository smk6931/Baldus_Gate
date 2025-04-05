// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "Engine/DataAsset.h"
#include "ItemDataTable.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UItemDataTable : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FItemStruct> ItemStruct;
};
