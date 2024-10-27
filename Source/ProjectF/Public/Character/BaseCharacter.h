// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTF_API ABaseCharacter : public ACharacter, public IHitInterFace
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)override;

	/*Combat*/
	virtual void Attack();
	virtual bool CanAttack();
	virtual void HandleDamage(float DamageAmount);
	UFUNCTION(BlueprintNativeEvent)
	void Die();
    bool IsAlive();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	void DisableMeshCollison();
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
	/*Anim Montages*/
	void StopAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayMontageSections(UAnimMontage* Montage, const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual int32 PlayDodgeMontage();
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAttributeComponents* Attributes;
	UPROPERTY(BluePrintReadOnly,VisibleAnywhere, Category = "Bug")
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;
	UPROPERTY(VisibleAnywhere, Category = "Bug")
	FVector WarpLocation;
	/**
	* Animation montages
	*/
	int32 PlayRandomMontageSections(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName>AttackMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName>DeathMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName>DodgeMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName>TwoHandAttackMontageSections;
	UPROPERTY(BluePrintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
private:
	/*
	* Components
	*/
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
public:
	FORCEINLINE TEnumAsByte<EDeathPose>GetDeathPose() const { return DeathPose;}
};