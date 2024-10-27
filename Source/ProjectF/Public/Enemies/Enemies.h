 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "Enemies.generated.h"
UCLASS()
class PROJECTF_API AEnemies : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemies();
	/*<AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void Destroyed() override;
	/*</AActor>*/
	/*<IHitInterFace>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)override;
	virtual void HandleDamage(float DamageAmount) override;
	bool IsDead();
protected:
	/*AActor*/
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SpawnDefaultWeapon();
	/*ABaseCharacter*/
	virtual void Die_Implementation() override;
	UFUNCTION(BlueprintCallable)
	void SpawnSoul();
	virtual void Attack() override;
	virtual bool CanAttack() override;	
	virtual void AttackEnd() override;
	
	UPROPERTY(BluePrintReadOnly,VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	EWeaponState WeaponState = EWeaponState::EWS_NoWeapon;

private:
	/*AI Behaviour Functions*/
	void IntializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void StartAttackTimer();
	void PatrolTimeFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void MoveToTarget(AActor* Target);
	UFUNCTION()
	void PawnSeen(APawn* SeePawn);
	bool IsOutSideCombatRadius();
	bool IsOutSideAttackRadius();
	bool IsInSideAttackRadius();
	bool IsAttacking();
	bool IsChasing();

	bool IsEngaged();
	bool InTargetRange(AActor* Target, double Radius);
	AActor* ChoosePatrolTarget();


	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASoul>SoulClass;
	/*AI Behaviour Variables*/
	UPROPERTY(EditAnywhere,Category = "Combat")
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere,Category = "Combat")
	float AcceptantRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 500.f;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax;
	float DeathLifeSpan = 3.f;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BluePrintReadWrite, Meta = (AllowPrivateAccess = true))
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;
	UPROPERTY()
	class AAIController* EnemyController;
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 500.f;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	float WaitMinTime = 5.f;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	float WaitMaxTime = 10.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChaseSpeed;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolSpeed;

	/*Components*/
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon>WeaponClasses;
};
