#pragma once
#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "EquippedOneHandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),

};
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_NoWeapon UMETA(DisplayName = "NoWeapon"),
	EWS_OneHand UMETA(DisplayName = "OneHand"),
	EWS_TwoHand UMETA(DisplayName = "TwoHand"),

};
UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead"),
	EAS_NotDead UMETA(DisplayName = "NotDead")
};
UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_MAX UMETA(DisplayName = "DefaultMax")
};
UENUM(BluePrintType)
enum class EEnemyState :uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Death  UMETA(DisplayName = "Death"),
	EES_Patrolling  UMETA(DisplayName = "Patrolling"),
	EES_Chasing  UMETA(DisplayName = "Chasing"),
	EES_Attacking  UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),

};