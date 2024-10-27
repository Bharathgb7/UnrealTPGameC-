// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces\PickUpInterface.h"
#include "ProjectFCharacter.generated.h"
class UInputMappingContext;
class UInputAction;
class ULookAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;
UCLASS()
class PROJECTF_API AProjectFCharacter : public ABaseCharacter,public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectFCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void Jump()override;
	void Dodge();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class ATreasure* Gold) override;
	void AddHealthPickup(AHealthPickUp* HealthPickUp) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*CallBacks For Input */
	UPROPERTY(EditAnywhere, Category = Input);
	UInputMappingContext* CharacterContext;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* MovementAction;
	void Move(const FInputActionValue& value);
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* LookMovement;
	void Look(const FInputActionValue& value);
	void EKeyPressed();
	void WeaponHandling();
	bool IsItTwoHand();
	bool IsItOneHand();
	void WeaponArm();
	virtual void Attack()override;
	void TwoHandWeaponAttack();
	void OneHandWeaponAttack();
	virtual void AttackEnd() override;
	virtual void DodgeEnd()override;

	/*Combat*/
	void EquipWeapon(AWeapon* Weapon);
	void TwoHandEquipState();
	void OneHandEquipState();
	void PlayEquipMontage(const FName& SectionName);
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Arm();
	void TwoHandWeaponArm();
	void OneHandWeaponArm();
	void DisArm();
	void TwoHandWeaponDisarm();
	void OneHandWeaponDisArm();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable)
	void FinishedEquipping();
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	UFUNCTION(BlueprintCallable)
	void PawnSeen(APawn* SeenPawn);
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsUnoccupied();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float CombatRadius=400.f;
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
private:
	void IntializeEchoOverlay();
	void InitialHUDParameters(class AProjectF_HUD* ProjectFHUD);
	void HUDIntialAttributes();
	void SetHUDHealth();
	bool IsUnOccuppied();
	/*CharacterComponents*/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* OverlappingWeapon;
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSense;
	/**
	*Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* TwoHandAttackMontage;
	
	/*Character States*/
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	EWeaponState WeaponState = EWeaponState::EWS_NoWeapon;
	class UEchoOverlay* EchoOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
