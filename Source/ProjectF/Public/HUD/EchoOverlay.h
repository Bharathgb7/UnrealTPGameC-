// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EchoOverlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UEchoOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);
	void SetStaminaPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSoul(int32 Soul);
	void SetHealthPickUp(int32 HealthPickUp);
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Stamina;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthPickupText;
};
