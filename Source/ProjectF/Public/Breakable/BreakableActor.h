// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterFace.h"
#include "BreakableActor.generated.h"
UCLASS()
class PROJECTF_API ABreakableActor : public AActor,public IHitInterFace
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UGeometryCollectionComponent* GeometryCollection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
private:	
	UPROPERTY(EditAnywhere,Category = "Breakable Properties")
	TArray< TSubclassOf<class ATreasure>> TreasureClasses;
	bool bBroken = false;
};
