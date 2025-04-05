// Copyright Epic Games, Inc. All Rights Reserved.

#include "E_BaldusGatePickUpComponent.h"
#include "E_BaldusGate/Character/E_BaldusGateCharacter.h"

UE_BaldusGatePickUpComponent::UE_BaldusGatePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UE_BaldusGatePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UE_BaldusGatePickUpComponent::OnSphereBeginOverlap);
}

void UE_BaldusGatePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AE_BaldusGateCharacter* Character = Cast<AE_BaldusGateCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
