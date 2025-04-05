// Copyright Epic Games, Inc. All Rights Reserved.

#include "E_BaldusGateCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PhysicsAssetRenderUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Engine/LocalPlayer.h"
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

    UE_LOG(LogTemp, Display, TEXT("발더스 캐릭터 태어났다"));
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
		UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 있음"))
		AddItemSlot();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 없음"))
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
	UE_LOG(LogTemplateCharacter, Display, TEXT("Item started"));
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
	PlayerItem = GetWorld()->SpawnActor<AItem>(PlayerItemFactory, GetActorLocation() + GetActorForwardVector()*50.0f, FRotator(0, 0, 0));
}

void AE_BaldusGateCharacter::CatchItemDrop()
{
    FHitResult hitinfo;
	FCollisionShape Shape;
	FCollisionQueryParams Params;

    DrawDebugBox(GetWorld(), GetActorLocation() + GetActorForwardVector() * 50, FVector(50,50,50),
    	FColor::Black, false, 0.5, 0, 0.5);
	
	bool GetItem = GetWorld()->SweepSingleByChannel(hitinfo, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50,
		FQuat::Identity,ECC_Visibility, FCollisionShape::MakeBox(FVector(50,50,50)), Params);
	if (GetItem)
	{
		AItem* CatchItem = Cast<AItem>(hitinfo.GetActor());
		if (CatchItem != nullptr)
		{
			UE_LOG(LogTemplateCharacter, Display, TEXT("캐릭터 Item 먹음"));
		}
	}
}

void AE_BaldusGateCharacter::AddItemSlot()
{
	class UInventorySlotUI* ItemSlot = CreateWidget<UInventorySlotUI>(GetWorld(),InventorySLotFactory);
	InventoryMenu->WBP_Inventory->BoxSlot->AddChildToWrapBox(ItemSlot);
}


