// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeapon.h"


// Sets default values
AItemWeapon::AItemWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemWeapon::BeginPlay()
{
	Super::BeginPlay();
	// ItemWeaponSpawn();
}

// Called every frame
void AItemWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemWeapon::ItemWeaponSpawn()
{
	RandomItemIndex = 0;
	// ItemComponent->SetStaticMesh(ItemMeshes[5]);
	ItemStruct.ItemTypeIndex = 1;
	ItemStruct.ItemIndex = 1;
}

void AItemWeapon::RandomItemDrop()
{
	if (ItemClientStruct.ItemMeshes.Num() > 0)
	{
		ItemComponent->SetStaticMesh(ItemClientStruct.ItemMeshes[5]);
		ItemStruct.ItemIndex = 5;
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Random Item Not Found%d"),ItemClientStruct.ItemMeshes.Num());
	}
}

