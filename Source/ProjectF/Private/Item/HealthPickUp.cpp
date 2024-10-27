// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HealthPickUp.h"
#include "Interfaces\PickUpInterface.h"

void AHealthPickUp::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->AddHealthPickup(this);
		SpawnPickupSystem();
		SpawnPickupSound();

	}
	Destroy();
}
