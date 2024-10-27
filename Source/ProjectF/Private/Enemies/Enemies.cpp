// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemies.h"
#include "Item/Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponents.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Item/Soul.h"
#include "Character/CharacterTypes.h"

// Sets default values
AEnemies::AEnemies()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);


}

void AEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

}

float AEnemies::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInSideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutSideAttackRadius())
	{
		ChaseTarget();
	}
	return DamageAmount;
}

void AEnemies::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemies::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	if (!IsDead())
	{
		ShowHealthBar();
	}
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
	if (IsInSideAttackRadius())
	{
		if (!IsDead())
		{
			StartAttackTimer();
		}
		
	}

}

void AEnemies::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemies::StartPatrolling, 1.f);
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemies::PawnSeen);
	}
	IntializeEnemy();
	Tags.Add(FName("Enemy"));
}

void AEnemies::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClasses)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClasses);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemies::Die_Implementation()
{
	Super::Die_Implementation();
	EnemyState = EEnemyState::EES_Death;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemies::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		const FVector SoulSpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
		ASoul* SpawnedSoul=World->SpawnActor<ASoul>(SoulClass, SoulSpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->GetOwner();
		}
		
	}
}

void AEnemies::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
	
}

bool AEnemies::CanAttack()
{
	bool bCanAttack =
		IsInSideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemies::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemies::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}
void AEnemies::IntializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();

}

void AEnemies::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMinTime, WaitMaxTime);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemies::PatrolTimeFinished, WaitTime);
	}
}

void AEnemies::CheckCombatTarget()
{
	if (IsOutSideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged())
		{
			StartPatrolling();
		}

	}
	else if (IsOutSideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			ChaseTarget();
		}

	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemies::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemies::Attack, AttackTime);
}

void AEnemies::PatrolTimeFinished()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemies::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemies::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemies::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemies::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemies::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemies::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemies::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemies::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptantRadius);
	EnemyController->MoveTo(MoveRequest);
}

void AEnemies::PawnSeen(APawn* SeePawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Death &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeePawn->ActorHasTag(FName("EngageableTarget"));
	if (bShouldChaseTarget)
	{
		CombatTarget = SeePawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

bool AEnemies::IsOutSideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemies::IsOutSideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemies::IsInSideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemies::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemies::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemies::IsDead()
{
	return EnemyState == EEnemyState::EES_Death;
}

bool AEnemies::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemies::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr)return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

AActor* AEnemies::ChoosePatrolTarget()
{
	TArray<AActor*>ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}















