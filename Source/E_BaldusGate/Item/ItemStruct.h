#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.Generated.h"

USTRUCT()
struct FItemStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString ItemName = FString("ItemName");

	UPROPERTY(EditAnywhere)
	FString ItemType = FString("ItemType");

	UPROPERTY(EditAnywhere)
	int32 Attack = 0;

	UPROPERTY(EditAnywhere)
	int32 Defense = 0;
};
