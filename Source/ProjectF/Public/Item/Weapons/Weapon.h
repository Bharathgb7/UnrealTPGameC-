// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Character/CharacterTypes.h"
#include "Weapon.generated.h"
class UBoxComponent;
class USoundBase;
/**
 * 
 */
UCLASS()
class PROJECTF_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner,APawn* NewInstigator);
	void DisableEmbersEffect();
	void DisableSphereCollison();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	TArray<AActor*>IgnoreActors;
	EWeaponState WeaponState = EWeaponState::EWS_NoWeapon;
protected:
	 virtual void BeginPlay() override;
	 void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	 void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	 UFUNCTION()
	 void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	 bool ActorIsSameType(AActor* OtherActor);
	 void ExceuteGetHit(FHitResult& BoxHit);
	 UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	 UBoxComponent* WeaponBox;
	 UFUNCTION(BlueprintImplementableEvent)
	 void CreateFields(const FVector& FieldLocation);
private:
	void BoxTrace(FHitResult& BoxHit);
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	FVector BoxTraceExtent = FVector(5.f);
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	bool bShowBoxDebug = false;
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	USoundBase* EquipSound;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	UPROPERTY(EditAnywhere, Category = "WeaponProperties");
	float Damage = 20.f;
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};
