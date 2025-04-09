// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "UObject/Object.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class E_BALDUSGATE_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
    FItemStruct ItemStructObject;
};
