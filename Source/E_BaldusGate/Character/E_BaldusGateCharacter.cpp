// Copyright Epic Games, Inc. All Rights Reserved.

#include "E_BaldusGateCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "JsonObjectConverter.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Engine/LocalPlayer.h"
#include "E_BaldusGate/Component/ItemComponent.h"
#include "E_BaldusGate/Item/Item.h"
#include "E_BaldusGate/UI/InventorySlotUI.h"
#include "E_BaldusGate/UI/InventoryUI.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AE_BaldusGateCharacter

AE_BaldusGateCharacter::AE_BaldusGateCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
}

void AE_BaldusGateCharacter::BeginPlay()
{
	Super::BeginPlay();
	InventoryMenu = CreateWidget<UInventoryMenu>(GetWorld(), InventoryMenuFactory);
}

//////////////////////////////////////////////////////////////////////////// Input

void AE_BaldusGateCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AE_BaldusGateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AE_BaldusGateCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AE_BaldusGateCharacter::Look);

		// Item
		EnhancedInputComponent->BindAction(ItemAction, ETriggerEvent::Started, this, &AE_BaldusGateCharacter::ItemInventory);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AE_BaldusGateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::E))
	{
		RandomItemDrop();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		AddItemSlot();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		CatchItemDrop();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		ExportStruct();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		ImportStruct();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
	{
		JsonToItem();
	}
}


void AE_BaldusGateCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AE_BaldusGateCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AE_BaldusGateCharacter::ItemInventory()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (flipflop == false)
	{
		InventoryMenu->AddToViewport();
		flipflop = true;
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		InventoryMenu->RemoveFromParent();
		flipflop = false;
		PlayerController->SetShowMouseCursor(false);
	}
}

void AE_BaldusGateCharacter::AttackWeapon()
{
	AItemWeapon* Weapon = GetWorld()->SpawnActor<AItemWeapon>(ItemWeaponFactory);
	Weapon->ItemComponent->SetSimulatePhysics(false);
	Weapon->ItemComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->ItemRoot->SetSimulatePhysics(false);
	Weapon->ItemRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocket"));
	// Weapon->SetActorLocation(FVector(5.431510,-13.000000,60.939114));
	// Weapon->SetActorRotation(FRotator(69.285781,223.025730,224.784160));
	// Weapon->SetActorScale3D(FVector(0.015));
}

void AE_BaldusGateCharacter::RandomItemDrop()
{
	PlayerItem = GetWorld()->SpawnActor<AItem>(PlayerItemFactory, GetActorLocation() + GetActorForwardVector()*25.0f, FRotator(0, 0, 0));
}

void AE_BaldusGateCharacter::CatchItemDrop()
{
    FHitResult hitinfo;
	FCollisionQueryParams Params;

    DrawDebugBox(GetWorld(), GetActorLocation() + GetActorForwardVector() * 50, FVector(25,25,25),
    	FColor::Black, false, 0.5, 0, 0.5);
	
	bool GetItem = GetWorld()->SweepSingleByChannel(hitinfo, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50,
		FQuat::Identity,ECC_Visibility, FCollisionShape::MakeBox(FVector(50,50,50)), Params);
	if (GetItem)
	{
		bool SameItem = false;
		AItem* Item = Cast<AItem>(hitinfo.GetActor());  if (Item != nullptr){
			// for (int32 i = 0 ; i < InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren().Num(); i++)
			// { UInventorySlotUI* Slot = Cast<UInventorySlotUI>(InventoryMenu->WBP_Inventory->BoxSlot->GetChildAt(i));}
			for (UWidget* Child : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
			{
				UInventorySlotUI* Slot = Cast<UInventorySlotUI>(Child);
				
				UE_LOG(LogTemp, Warning, TEXT("먹은 아이템 Index: %d / 슬롯 Index: %d / 슬롯 순서 %d" ), 
				 Item->ItemStruct.ItemIndex, Slot->ItemStruct.ItemIndex, InventoryMenu->WBP_Inventory->BoxSlot->GetChildIndex(Slot));
				
				if (Slot != nullptr && Slot->ItemStruct.ItemIndex == Item->ItemStruct.ItemIndex)
				{
					Slot->ItemStruct.ItemNum++;
					// 아이템 넘기는게 과연좋은지 아중에 확인해보자
					Slot->Item = Item;
					Slot->ItemCount->SetText(FText::AsNumber(Slot->ItemStruct.ItemNum));
					SameItem = true;
					break;
				}
			}
			if (SameItem == false)
			{
				UInventorySlotUI* Slot = CreateWidget<UInventorySlotUI>(GetWorld(), InventorySLotFactory);
				FSlateBrush Brush;
				Brush.SetResourceObject(Item->ItemStruct.ItemTextures[Item->ItemStruct.ItemIndex]);
				Slot->ItemIconImage->SetBrush(Brush);
				Slot->ItemStruct = Item->ItemStruct;
				InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(Slot);
			}
		}
		Item->Destroy();
	}
}

void AE_BaldusGateCharacter::AddItemSlot()
{
	class UInventorySlotUI* ItemSlot = CreateWidget<UInventorySlotUI>(GetWorld(),InventorySLotFactory);
	InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(ItemSlot);
}

void AE_BaldusGateCharacter::ExportStruct() //2번
{
	// for (int32 i = 0; i < InventoryMenu->WBP_Inventory->BoxSlot->GetChildrenCount() ; i++)
	// {
	// 	UInventorySlotUI* SlotUI = Cast<UInventorySlotUI>(InventoryMenu->WBP_Inventory->BoxSlot->GetChildAt(i));
	// 	ItemStructArray.Add(SlotUI->ItemStruct);
	// 	// UE_LOG(LogTemp,Warning,TEXT("구조체 순서%d / 구조체 인덱스%d"),i,ItemStructArray[i].ItemIndex)
	// 	FJsonObjectConverter::UStructToJsonObjectString(ItemStructArray[i],jsonString);
	// }
	
	int32 index = InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren().Num()-1;
	UInventorySlotUI* SlotUI = Cast<UInventorySlotUI>(InventoryMenu->WBP_Inventory->BoxSlot->GetChildAt(index));
	
	FJsonObjectConverter::UStructToJsonObjectString(SlotUI->ItemStruct,jsonString);
	UE_LOG(LogTemp,Warning,TEXT("구조체 -> 제이슨 스트링 : %s"),*jsonString);
}

void AE_BaldusGateCharacter::ImportStruct()//3번
{
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString,&MyItemStruct);
	UE_LOG(LogTemp,Warning,TEXT("캐릭터 제이슨 아이템 가져오기 : %i"),MyItemStruct.ItemIndex);
}


void AE_BaldusGateCharacter::JsonToItem()// 4번
{
	AItem* JsonItem = GetWorld()->SpawnActor<AItem>(PlayerItemFactory, GetActorLocation() + GetActorForwardVector()*25.0f, FRotator(0, 0, 0));

	JsonItem->ItemStruct = MyItemStruct;
	JsonItem->ItemComponent->SetStaticMesh(JsonItem->ItemMeshes[JsonItem->ItemStruct.ItemIndex]);
    UE_LOG(LogTemp,Warning,TEXT("캐릭터 제이슨 아이템 소환 %i"),JsonItem->ItemStruct.ItemIndex);
}

