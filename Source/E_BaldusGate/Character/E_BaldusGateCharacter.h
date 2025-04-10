// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "E_BaldusGate/UI/InventoryMenu.h" // 이거 꼭 있어야 함
#include "E_BaldusGate/Item/Item.h"
#include "E_BaldusGate/Component/ItemComponent.h"
#include "E_BaldusGate/Item/ItemList/ItemWeapon.h"
#include "E_BaldusGateCharacter.generated.h"
// 아이템 ui 컨트룰러 '

// 게임스테이트는 플레이어정보 뺴고 
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AE_BaldusGateCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	//** Jump Input Action *//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ItemAction;

public:
	AE_BaldusGateCharacter();

	UPROPERTY(EditAnywhere,Category = UI)
	class UInventoryMenu* InventoryMenu;
	
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UInventoryMenu> InventoryMenuFactory;

	bool flipflop = false;

	UPROPERTY(EditAnywhere)
	TArray<FItemStruct> ItemStructArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	FItemStruct MyItemStruct;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TArray<FItemStruct> MyItemStructArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TArray<FString> MyJsonArray;

	UPROPERTY(EditAnywhere)
	FString jsonString;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Item)
	class AItem* PlayerItem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TSubclassOf<class AItem> PlayerItemFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TSubclassOf<class UInventorySlotUI> InventorySLotFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TArray<UTexture2D*> ItemTextures;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
    TArray<int32> SlotIndexArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Component)
	class UItemComponent* ItemComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Component)
	TSubclassOf<AItemWeapon> ItemWeaponFactory;
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ItemInventory();

	UFUNCTION(Exec)
	void PrintSlotUI();
protected:
	// APawn interface
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaTime) override;
	// End of APawn interface

public:
	// /** Returns Mesh1P subobject **/
	// USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void AttackWeapon();
	
	void RandomItemDrop();

	void CatchItemDrop();

	void CatchImageItem();

	void AddItemSlot();


	void ExportStructArray();

	void ImportStructArray();

	void JsonToItemArray();

	void SaveJsonString();

};

