// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEchoOverlay::SetHealthPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UEchoOverlay::SetStaminaPercent(float Percent)
{
	if (Stamina)
	{
		Stamina->SetPercent(Percent);
	}
}

void UEchoOverlay::SetGold(int32 Gold)
{
	const FString String = FString::Printf(TEXT("%d"),Gold);
	const FText Text = FText::FromString(String);
	GoldText->SetText(Text);
}

void UEchoOverlay::SetSoul(int32 Soul)
{

	const FString String = FString::Printf(TEXT("%d"),Soul);
	const FText Text = FText::FromString(String);
	SoulsText->SetText(Text);
}

void UEchoOverlay::SetHealthPickUp(int32 HealthPickUp)
{
	const FString String = FString::Printf(TEXT("%d"), HealthPickUp);
	const FText Text = FText::FromString(String);
	HealthPickupText->SetText(Text);
}
