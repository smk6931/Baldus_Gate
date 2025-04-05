#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.Generated.h"

USTRUCT()
struct FItemStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString ItemName;

	UPROPERTY(EditAnywhere)
	FString ItemType;

	UPROPERTY(EditAnywhere)
	int32 Attack;

	UPROPERTY(EditAnywhere)
	int32 Defense;
};
