// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponents.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTF_API UAttributeComponents : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponents();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenRateStamina(float DeltaTime);
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 HealthPickUp;
public:
	void RecieveDamage(float Damage);
	float UpdateDamagedHealth();
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	void AddHealthPickup(int32 NumberOfPickUps);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetHealthPickUpAmount() const{ return HealthPickUp;}
	FORCEINLINE float GetDodgeCost() const {return DodgeCost;}
	FORCEINLINE float GetStamina() const { return Stamina; }
};
