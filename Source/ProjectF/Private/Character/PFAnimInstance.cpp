// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PFAnimInstance.h"
#include "Character/ProjectFCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AProjectFCharacter>(TryGetPawnOwner());
	if (Character)
	{
		ProjectFCharacterMovementComponent = Character->GetCharacterMovement();
	}
}

void UPFAnimInstance::NativeUpdateAnimation(float Deltatime)
{
	Super::NativeUpdateAnimation(Deltatime);
	if (ProjectFCharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ProjectFCharacterMovementComponent->Velocity);
		IsFalling = ProjectFCharacterMovementComponent->IsFalling();
		CharacterState = Character->GetCharacterState();
		ActionState = Character->GetActionState();
		DeathPose = Character->GetDeathPose();
	}
}
