// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "HealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API AHealthPickUp : public AItem
{
	GENERATED_BODY()
public:
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = "Health");
	int32 Health=1;
public:
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(int32 NumberOfHealth) { Health = NumberOfHealth; }
};
