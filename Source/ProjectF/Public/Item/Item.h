// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces\PickUpInterface.h"
#include "Item.generated.h"
class USphereComponent; 
class UStaticMeshComponent;
enum class EItemState :uint8
{
	EIS_Hovering,
	EIS_Equipped
};
UCLASS()
class PROJECTF_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters");
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters");
	float TimeConstant = 5.f;
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	template<typename T>
	T Avg(T First, T Second);
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	EItemState ItemState = EItemState::EIS_Hovering;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	/*Niagara Effects*/
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmbersEffect;
	UPROPERTY(EditAnyWhere)
	class UNiagaraSystem* PickupEffect;
	UPROPERTY(EditAnyWhere)
	class USoundBase* PickupSound;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float RunningTime;
	
};
template<typename T>
inline T AItem::Avg(T First, T Second)
{

	return T(First + Second) / 2;
}
