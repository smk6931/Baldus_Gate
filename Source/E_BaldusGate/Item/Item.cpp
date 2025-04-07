// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "E_BaldusGate/Character/E_BaldusGateCharacter.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemRoot = CreateDefaultSubobject<UBoxComponent>("ItemRoot");
	SetRootComponent(ItemRoot);
	ItemComponent = CreateDefaultSubobject<UStaticMeshComponent>("ItemComponent");
	ItemComponent->SetupAttachment(ItemRoot);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// ItemComponent->SetSimulatePhysics(true);
	
	RandomItemDrop();

	ItemComponent->OnComponentBeginOverlap.AddDynamic(this,&AItem::OnMyBeginOverlapped);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::OnMyBeginOverlapped(UPrimitiveComponent* OverlaeppedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwimming, const FHitResult& SwimmingResult)
{
	
}

void AItem::RandomItemDrop()
{
	ItemIndex = FMath::RandRange(0,ItemMeshes.Num()-1);
	ItemComponent->SetStaticMesh(ItemMeshes[ItemIndex]);
	ItemStruct.ItemIndex = ItemIndex;
}

