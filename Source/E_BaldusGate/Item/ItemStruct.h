#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.Generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) // 아이템 상세종류
    int32 ItemIndex = 0;

	UPROPERTY(EditAnywhere)
	int32 ItemTypeIndex = 0; // 아이템 종류
	 
	UPROPERTY(EditAnywhere)
	FString ItemName = FString("ItemName");

	UPROPERTY(EditAnywhere)
	FString ItemType = FString("ItemType");

	UPROPERTY(EditAnywhere)
	int32 Attack = 0;

	UPROPERTY(EditAnywhere)
	int32 Defense = 0;

	UPROPERTY(EditAnywhere)
	int32 ItemNum = 0;

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*>ItemTextures;
};
