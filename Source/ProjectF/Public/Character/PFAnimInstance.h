// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "PFAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API UPFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltatime) override;
	UPROPERTY(BlueprintReadOnly)
	class AProjectFCharacter* Character;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* ProjectFCharacterMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;
	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	ECharacterState CharacterState;
	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	EActionState ActionState;
	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	TEnumAsByte<EDeathPose> DeathPose;
};

