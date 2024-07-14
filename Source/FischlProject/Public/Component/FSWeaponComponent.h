// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSFunctionLibrary.h"
#include "FSWeaponComponent.generated.h"

class UNiagaraSystem;

/**
 * Weapon handled by a character.
 */
UCLASS()
class FISCHLPROJECT_API UFSWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UFSWeaponComponent();

protected:
	/** Parameters when damaging the enemy */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FDamageParam DamageParam;

	/** Called when overlapping enemies. */
	UFUNCTION(BlueprintCallable)
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	//~ Begin UActorComponent Interface.
	virtual void InitializeComponent() override;
	//~ End UActorComponent Interface.

private:
	/** Actors that have been damaged. Used to make each character damaged only once. */
	TArray<AActor*> DamagedActors;
};
