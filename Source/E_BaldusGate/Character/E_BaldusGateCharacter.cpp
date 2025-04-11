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
		// CatchItemDrop();
		CatchImageItem();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
	{
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
	{
		ExportStructArray();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
	{
		ImportStructArray();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	{
		JsonToItemArray();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		SaveJsonString();
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

void AE_BaldusGateCharacter::ItemInventory() // I 인벤토리 생성
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

void AE_BaldusGateCharacter::AttachWeapon(AItemWeapon* Weapon)
{
	// AItemWeapon* Weapon = GetWorld()->SpawnActor<AItemWeapon>(ItemWeaponFactory);
	Weapon->ItemComponent->SetSimulatePhysics(false);
	Weapon->ItemComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->ItemRoot->SetSimulatePhysics(false);
	Weapon->ItemRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
	Weapon->SetActorRelativeLocation(FVector(5.431510,-13.000000,60.939114));
	Weapon->SetActorRelativeRotation(FRotator(69.285781,223.025730,224.784160));
}

void AE_BaldusGateCharacter::RandomItemDrop() // E 키
{
	PlayerItem = GetWorld()->SpawnActor<AItem>(PlayerItemFactory, GetActorLocation() + GetActorForwardVector()*25.0f, FRotator(0, 0, 0));
}

void AE_BaldusGateCharacter::CatchImageItem() // 1번
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
			for (UWidget* Child : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
			{
				UInventorySlotUI* Slot = Cast<UInventorySlotUI>(Child);
				// UE_LOG(LogTemp,Warning,TEXT("SlotIndex = %d"),InventoryMenu->WBP_Inventory->BoxSlot->GetChildIndex(Child));
				// UE_LOG(LogTemp, Warning, TEXT("먹은 아이템 Index: %d / 슬롯 Index: %d / 슬롯 순서 %d" ), 
				//  Item->ItemStruct.ItemIndex, Slot->ItemStruct.ItemIndex, InventoryMenu->WBP_Inventory->BoxSlot->GetChildIndex(Slot));
				if (Slot != nullptr && Slot->ItemStruct.ItemIndex == Item->ItemStruct.ItemIndex)
				{
					Slot->ItemStruct.ItemNum++;
					Slot->ItemCount->SetText(FText::AsNumber(Slot->ItemStruct.ItemNum));
					SameItem = true;
					break;
				}
			}
			if (SameItem == false)
			{
				for (UWidget* Child : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
				{
					auto* SlotUi = Cast<UInventorySlotUI>(Child);
					if (SlotUi->ItemStruct.ItemIndex == -1)
					{
						UE_LOG(LogTemp,Warning,TEXT("캐릭터 SlotIndex = %d"),InventoryMenu->WBP_Inventory->BoxSlot->GetChildIndex(SlotUi));
						FSlateBrush Brush;
						SlotUi->ItemStruct = Item->ItemStruct;
						SlotUi->ItemClientStruct = Item->ItemClientStruct;
						Brush.SetResourceObject(SlotUi->ItemClientStruct.ItemTextures[Item->ItemStruct.ItemIndex]);
						SlotUi->ItemIconImage->SetBrush(Brush);
						break;
					}
				}
				UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 없음"))
			}
		}
		Item->Destroy();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("캐릭터 아이템 못먹음"));
	}
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
			for (UWidget* Child : InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren())
			{
				UInventorySlotUI* Slot = Cast<UInventorySlotUI>(Child);
				
				UE_LOG(LogTemp, Warning, TEXT("먹은 아이템 Index: %d / 슬롯 Index: %d / 슬롯 순서 %d" ), 
				 Item->ItemStruct.ItemIndex, Slot->ItemStruct.ItemIndex, InventoryMenu->WBP_Inventory->BoxSlot->GetChildIndex(Slot));
				
				if (Slot != nullptr && Slot->ItemStruct.ItemIndex == Item->ItemStruct.ItemIndex)
				{
					Slot->ItemStruct.ItemNum++;
					// 아이템 넘기는게 과연좋은지 아중에 확인해보자
					Slot->ItemCount->SetText(FText::AsNumber(Slot->ItemStruct.ItemNum));
					SameItem = true;
					break;
				}
			}
			if (SameItem == false)
			{
				UInventorySlotUI* Slot = CreateWidget<UInventorySlotUI>(GetWorld(), InventorySLotFactory);
				FSlateBrush Brush;
				// Brush.SetResourceObject(Item->ItemStruct.ItemTextures[Item->ItemStruct.ItemIndex]);
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

void AE_BaldusGateCharacter::ExportStructArray() // 5번 슬롯 아이템 -> 구조체 배열 넣기
{
	for (int32 i = 0; i < InventoryMenu->WBP_Inventory->BoxSlot->GetAllChildren().Num(); i++)
	{
		UInventorySlotUI* SlotUi = Cast<UInventorySlotUI>(InventoryMenu->WBP_Inventory->BoxSlot->GetChildAt(i));
		UE_LOG(LogTemp,Warning,TEXT("5번 슬롯 -> 구조체 인덱스 %i"),SlotUi->ItemStruct.ItemIndex);
		ItemStructArray.Add(SlotUi->ItemStruct);
	}
}

void AE_BaldusGateCharacter::ImportStructArray() // 6번 구조체 배열-> 제이슨
{
	for (int32 i = 0; i < ItemStructArray.Num(); i++)
	{
		FJsonObjectConverter::UStructToJsonObjectString
		(ItemStructArray[i],jsonString);MyJsonArray.Add(jsonString);
		UE_LOG(LogTemp,Warning,TEXT("6번 제이슨 인덱스%s"),*jsonString);
	}
}

void AE_BaldusGateCharacter::JsonToItemArray() // 7번 제이슨 -> 구조체 배열 및 소환
{
	for (int32 i = 0; i < MyJsonArray.Num(); i++)
	{
		AItem* Item = GetWorld()->SpawnActor<AItem>(PlayerItemFactory,GetActorLocation() + GetActorForwardVector() * 100,
			FRotator(0,0,0));
		Item->ItemRoot->SetSimulatePhysics(true);
		FJsonObjectConverter::JsonObjectStringToUStruct(MyJsonArray[i],&MyItemStruct);
		MyItemStructArray.Add(MyItemStruct);
		Item->ItemStruct = MyItemStructArray[i];
		// Item->ItemComponent->SetStaticMesh(Item->ItemMeshes[Item->ItemStruct.ItemIndex]);
		UE_LOG(LogTemp,Warning,TEXT("7번 제이슨 -> 구조체 아이템 인덱스%d"),MyItemStructArray[i].ItemIndex);
	}
}

void AE_BaldusGateCharacter::SaveJsonString() // 8번 제이슨 파일 저장
{
	for (int32 i = 0; i < MyJsonArray.Num(); i++)
	{
		FString FileName = FString::Printf(TEXT("%d_test.txt"),i);
		FString path = FString::Printf(TEXT("%s%s"),*FPaths::ProjectDir(),*FileName);
		FFileHelper::SaveStringToFile(MyJsonArray[i],*path);
	}
}

void AE_BaldusGateCharacter::PrintSlotUI()
{
	
}
