// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponents.h"

// Sets default values for this component's properties
UAttributeComponents::UAttributeComponents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

}


// Called when the game starts
void UAttributeComponents::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UAttributeComponents::RecieveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponents::UpdateDamagedHealth()
{
	return Health = FMath::Clamp(Health + MaxHealth, 0.f, MaxHealth);
}

void UAttributeComponents::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

float UAttributeComponents::GetHealthPercent()
{
	return Health/MaxHealth;
}

float UAttributeComponents::GetStaminaPercent()
{
	return Stamina/MaxStamina;
}

bool UAttributeComponents::IsAlive()
{
	return Health > 0;
}

void UAttributeComponents::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

void UAttributeComponents::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

void UAttributeComponents::AddHealthPickup(int32 NumberOfPickUps)
{
	HealthPickUp += NumberOfPickUps;
}


// Called every frame
void UAttributeComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponents::RegenRateStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

