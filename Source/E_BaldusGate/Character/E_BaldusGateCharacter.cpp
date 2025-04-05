// Copyright Epic Games, Inc. All Rights Reserved.

#include "E_BaldusGateCharacter.h"

#include "EditorDirectories.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PhysicsAssetRenderUtils.h"
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
	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

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
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		AddItemSlot();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		CatchItemDrop();
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
		AItem* Item = Cast<AItem>(hitinfo.GetActor());
		if (Item != nullptr)
		{
			for (int32 i = 0; i < InventoryMenu->WBP_Inventory->BoxSlot->GetChildrenCount() ; i++)
			{
				// UE_LOG(LogTemp,Warning,TEXT("캐릭터 아이템창 슬롯 갯수%d 현재 아이템창 인덱스 값%d"),i,ItemComponent->ItemCompStruct[i].ItemIndex);
				UInventorySlotUI* Slot = Cast<UInventorySlotUI>(InventoryMenu->WBP_Inventory->BoxSlot->GetChildAt(i));
				if (ItemComponent->ItemCompStruct[i].ItemIndex == Item->ItemIndex)
				{
					// UE_LOG(LogTemp,Warning,TEXT("캐릭터 아이템창 슬롯 갯수%d 주운 아이템 인덱스%d"),i,Item->ItemStruct.ItemIndex);
					ItemComponent->ItemCompStruct[i].ItemNum++;
					Slot->ItemCount->SetText(FText::AsNumber(ItemComponent->ItemCompStruct[i].ItemNum));
				}
			}
			for (UWidget* Slot : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
			{
				if (SlotIndexArray.Contains(Item->ItemStruct.ItemIndex))
				{
					SameItem = true;
					break;
				}
			}
		}
		if (SameItem == false)
		{
			SlotIndexArray.Add(Item->ItemStruct.ItemIndex);
			FSlateBrush Brush;
			UInventorySlotUI* Slot = CreateWidget<UInventorySlotUI>(GetWorld(), InventorySLotFactory);
			Brush.SetResourceObject(ItemTextures[Item->ItemStruct.ItemIndex]);
			Slot->ItemIconImage->SetBrush(Brush);
			InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(Slot);
			//아이템 컴포넌트 스트럭트 갯수증가
			ItemComponent->ItemCompStruct.Add(Item->ItemStruct);
			
			for (int32 i = 0 ; i < SlotIndexArray.Num() ; i++)
			{
				UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 창 갯수%d index%d"),SlotIndexArray.Num(),SlotIndexArray[i])
			}
		}
		Item->Destroy();
		
		// bool bAlreadyHasItem = false;
			//
			// for (UWidget* Widget : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
			// {
			// 	UInventorySlotUI* Slot = Cast<UInventorySlotUI>(Widget);
			// 	UInventorySlotUI* CastedSlot = Cast<UInventorySlotUI>(Slot);
			// 	
			// 	if (CastedSlot && SlotIndexArray.Contains(CatchItem->ItemStruct.ItemIndex))
			// 	{
			// 		UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 차일드 갯수%d"),InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren().Num());
			// 		bAlreadyHasItem = true;
			// 		break;
			// 	}
			// }
			//
			// // 중복 아니면 새 슬롯 추가
			// if (!bAlreadyHasItem)
			// {
			// 	UInventorySlotUI* ItemSlot = CreateWidget<UInventorySlotUI>(GetWorld(), InventorySLotFactory);
			// 	FSlateBrush Brush;
			// 	Brush.SetResourceObject(ItemTextures[CatchItem->ItemStruct.ItemIndex]);
			// 	
			// 	ItemSlot->ItemIconImage->SetBrush(Brush);
			// 	SlotIndexArray.Add(CatchItem->ItemStruct.ItemIndex);
			// 	for (int32 i = 0 ; i < SlotIndexArray.Num() ; i++)
			// 	{
			// 		UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 창 갯수%d index%d"),SlotIndexArray.Num(),SlotIndexArray[i])
			// 	}
			// 	InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(ItemSlot);
			// }
	}
}

void AE_BaldusGateCharacter::AddItemSlot()
{
	class UInventorySlotUI* ItemSlot = CreateWidget<UInventorySlotUI>(GetWorld(),InventorySLotFactory);
	InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(ItemSlot);
}


