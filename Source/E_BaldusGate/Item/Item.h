// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ItemStruct.h"
#include "Item.generated.h"

UCLASS()
class E_BALDUSGATE_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	FItemStruct ItemStruct;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> ItemMeshes;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* ItemComponent;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* ItemRoot;

	UPROPERTY(EditAnywhere)
	int32 ItemIndex = 0;

	UFUNCTION()
	void OnMyBeginOverlapped(UPrimitiveComponent* OverlaeppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSwimming, const FHitResult& SwimmingResult);

	void RandomItemDrop();
};
