// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectF_HUD.generated.h"
class UEchoOverlay;
UCLASS()
class PROJECTF_API AProjectF_HUD : public AHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectF")
	TSubclassOf<UEchoOverlay>EchoOverlayClass;
	UPROPERTY()
	UEchoOverlay* EchoOverlay;
protected:
	virtual void BeginPlay() override;
public:
	FORCEINLINE UEchoOverlay* GetEchoOverlay() const { return EchoOverlay;}
};
