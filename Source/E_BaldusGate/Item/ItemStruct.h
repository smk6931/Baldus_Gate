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

	UPROPERTY(EditAnywhere) // 아이템 종류
	int32 ItemTypeIndex = 0; 
	 
	UPROPERTY(EditAnywhere) // 아이템 이름
	FString ItemName = FString("ItemName");

	UPROPERTY(EditAnywhere) // 아이템 분류이름
	FString ItemType = FString("ItemType");

	UPROPERTY(EditAnywhere) // 아이템 상세설명
	FString ItemDetail = FString("ItemDetail");

	UPROPERTY(EditAnywhere) // 아이템 갯수
	int32 ItemNum = 0;

	UPROPERTY(EditAnywhere) // 장비 아이템 공격력
	int32 Attack = 0;

	UPROPERTY(EditAnywhere) // 소모 HP 상승
	int32 HPup = 0;

	UPROPERTY(EditAnywhere) // 아이템 텍스쳐
	TArray<UTexture2D*>ItemTextures;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*>ItemMeshes;
};
