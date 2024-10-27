// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ProjectF_HUD.h"
#include "HUD/EchoOverlay.h"

void AProjectF_HUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController=World->GetFirstPlayerController();
		if (PlayerController && EchoOverlayClass)
		{
		    EchoOverlay =CreateWidget<UEchoOverlay>(PlayerController, EchoOverlayClass);
			EchoOverlay->AddToViewport();
		}
	}
}
