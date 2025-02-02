// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime)override;
protected:
	virtual void BeginPlay();
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override ;
private:
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;
	float DesiredZ;
	double DriftRate = -15.f;
public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
