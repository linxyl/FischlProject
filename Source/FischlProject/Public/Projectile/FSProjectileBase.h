// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSFunctionLibrary.h"
#include "FSProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class AFSCharacter;
class UNiagaraSystem;

UCLASS()
class FISCHLPROJECT_API AFSProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSProjectileBase();

protected:
	/** Used to collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	/** Movement component used for movement logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	/** Effect when spawning */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* SpawnVFX;

	/** Effect when exploding */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* ExplodeVFX;

	/** Damage parameter caused to target */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FDamageParam Param;

	/** Whether to destroy self immediately after damaging the target */
	UPROPERTY(EditDefaultsOnly)
	bool bDamageOnce;

	/** Called when hitting enemy. */
	void Explode(AActor* Target);

	AActor* Target;

protected:
	/** Called when overlapping actors. */
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/** Actors ignored in collision. */
	TArray<AActor*> IgnoredActors;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	//~ End AActor Interface
};
