// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MyPawn.generated.h"
class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class UPawnMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTF_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input);
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input);
	UInputAction* MoveAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input);
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere);
	UPawnMovementComponent* MovementComponent;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;


};
