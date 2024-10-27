// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ProjectFCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponents.h"
#include "Item/Weapons//Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/ProjectF_HUD.h"
#include "HUD/EchoOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Soul.h"
#include "Item/Treasure.h"
#include "Item/HealthPickUp.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemies/Enemies.h"

AProjectFCharacter::AProjectFCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(SpringArm);
	
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("EyeBrows");

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void AProjectFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Tags.Add(FName("EngageableTarget"));
	if (Attributes && EchoOverlay)
	{
		Attributes->RegenRateStamina(DeltaTime);
		EchoOverlay->SetStaminaPercent(Attributes->GetStaminaPercent());
	}
}

void AProjectFCharacter::Jump()
{
	if (IsUnOccuppied())
	{
		Super::Jump();
	}
}

void AProjectFCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT(" dodge () Called"))
	if (IsUnoccupied() || !HasEnoughStamina())
	{
		return;
	}
	PlayDodgeMontage();
	if (Attributes && EchoOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		EchoOverlay->SetStaminaPercent(Attributes->GetStaminaPercent());
	}
	ActionState = EActionState::EAS_Dodge;
	
}

bool AProjectFCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina()>Attributes->GetDodgeCost();
}

bool AProjectFCharacter::IsUnoccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void AProjectFCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterContext, 0);
		}
	}
	Tags.Add(FName("EngageableTarget"));
	IntializeEchoOverlay();
	if (PawnSense)
	{
		PawnSense->OnSeePawn.AddDynamic(this, &AProjectFCharacter::PawnSeen);
	}
}

void AProjectFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AProjectFCharacter::Move);
		EnhancedInputComponent->BindAction(LookMovement, ETriggerEvent::Triggered, this, &AProjectFCharacter::Look);
		PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AProjectFCharacter::Jump);
		PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AProjectFCharacter::EKeyPressed);
		PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AProjectFCharacter::Attack);
		PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AProjectFCharacter::Dodge);
	}
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AProjectFCharacter::MoveForward);
}

void AProjectFCharacter::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}

}

float AProjectFCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AProjectFCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AProjectFCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && EchoOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		EchoOverlay->SetSoul(Attributes->GetSouls());
	}
	
}

void AProjectFCharacter::AddGold(ATreasure* Gold)
{
	if (Attributes && EchoOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		EchoOverlay->SetGold(Attributes->GetGold());
	}
}

void AProjectFCharacter::AddHealthPickup(AHealthPickUp* HealthPickUp)
{
	if (Attributes && EchoOverlay)
	{
		Attributes->AddHealthPickup(HealthPickUp->GetHealth());
		EchoOverlay->SetHealthPickUp(Attributes->GetHealthPickUpAmount());
		EchoOverlay->SetHealthPercent(Attributes->UpdateDamagedHealth());
	}
	
}

void AProjectFCharacter::Move(const FInputActionValue& value)
{
	if (ActionState != EActionState::EAS_Unoccupied)return;
	{
		const FVector2D MovementVector = value.Get<FVector2D>();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void AProjectFCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AProjectFCharacter::EKeyPressed()
{
	WeaponHandling();
	
}

void AProjectFCharacter::WeaponHandling()
{
	OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (IsItOneHand())
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else if (IsItTwoHand())
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		WeaponArm();
	}
}

bool AProjectFCharacter::IsItTwoHand()
{
	return OverlappingWeapon && OverlappingWeapon->ActorHasTag(FName("Weapon2"));
}

bool AProjectFCharacter::IsItOneHand()
{
	return OverlappingWeapon && OverlappingWeapon->ActorHasTag(FName("Weapon1"));
}

void AProjectFCharacter::WeaponArm()
{
	if (CanDisarm())
	{
		DisArm();
	}
	else if (CanArm())
	{
		Arm();
	}
}

void AProjectFCharacter::Attack()
{
	//still able to attack even if the combat target is nullptr and have attack() to ensure the ct is cleared
	if (CanAttack() && IsItOneHand())
	{
		OneHandWeaponAttack();
	}
	else if (CanAttack())
	{
		TwoHandWeaponAttack();
	}
	
		
}

void AProjectFCharacter::TwoHandWeaponAttack()
{	
	PlayRandomMontageSections(TwoHandAttackMontage, TwoHandAttackMontageSections);
	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	ActionState = EActionState::EAS_Attacking;	
}

void AProjectFCharacter::OneHandWeaponAttack()
{
	PlayAttackMontage();
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_Attacking;
		
}

void AProjectFCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AProjectFCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

void AProjectFCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	EquippedWeapon = Weapon;
	if (EquippedWeapon->ActorHasTag(FName("Weapon1")))
	{
		OneHandEquipState();
	}
	else
	{
		TwoHandEquipState();
	}
	OverlappingItem = nullptr;
}

void AProjectFCharacter::TwoHandEquipState()
{
	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	WeaponState = EWeaponState::EWS_TwoHand;
}

void AProjectFCharacter::OneHandEquipState()
{
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	WeaponState = EWeaponState::EWS_OneHand;
}

void AProjectFCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool AProjectFCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState > ECharacterState::ECS_Unequipped;
}

bool AProjectFCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState > ECharacterState::ECS_Unequipped;
}

bool AProjectFCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		WeaponState >=EWeaponState::EWS_OneHand &&
		EquippedWeapon;
}

void AProjectFCharacter::Arm()
{
	if (WeaponState == EWeaponState::EWS_OneHand)
	{
		OneHandWeaponArm();
	}
	else
	{
		TwoHandWeaponArm();
	}	
}

void AProjectFCharacter::TwoHandWeaponArm()
{
	PlayEquipMontage(FName("EquipTwoHand"));
	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AProjectFCharacter::OneHandWeaponArm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
		
}

void AProjectFCharacter::DisArm()
{
	if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)
	{
		OneHandWeaponDisArm();
	}
	else
	{
		TwoHandWeaponDisarm();
	}
		
}

void AProjectFCharacter::TwoHandWeaponDisarm()
{
	PlayEquipMontage(FName("UnEquipTwoHand"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
	WeaponState = EWeaponState::EWS_TwoHand;
}

void AProjectFCharacter::OneHandWeaponDisArm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
	WeaponState = EWeaponState::EWS_OneHand;
}

void AProjectFCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_Weapon_Socket"));
	}
}

void AProjectFCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AProjectFCharacter::FinishedEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AProjectFCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AProjectFCharacter::PawnSeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Seen before anything"));
	UE_LOG(LogTemp, Warning, TEXT("PAwn Seen"));
	TArray<APawn*>Enemies;
	Enemies.AddUnique(SeenPawn);
	for (APawn* EnemyPawn : Enemies)
	{
		CombatTarget = EnemyPawn;
	}

}


void AProjectFCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollison();
}

void AProjectFCharacter::IntializeEchoOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AProjectF_HUD* ProjectFHUD = Cast<AProjectF_HUD>(PlayerController->GetHUD());
		if (ProjectFHUD)
		{
			InitialHUDParameters(ProjectFHUD);
		}

	}
}

void AProjectFCharacter::InitialHUDParameters(AProjectF_HUD* ProjectFHUD)
{
	EchoOverlay = ProjectFHUD->GetEchoOverlay();
	if (EchoOverlay && Attributes)
	{
		HUDIntialAttributes();
	}
}

void AProjectFCharacter::HUDIntialAttributes()
{
	EchoOverlay->SetHealthPercent(Attributes->GetHealthPercent());
	EchoOverlay->SetStaminaPercent(1.f);
	EchoOverlay->SetGold(0);
	EchoOverlay->SetSoul(0);
	EchoOverlay->SetHealthPickUp(0);
}

void AProjectFCharacter::SetHUDHealth()
{
	if (EchoOverlay && Attributes)
	{
		EchoOverlay->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

bool AProjectFCharacter::IsUnOccuppied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}



