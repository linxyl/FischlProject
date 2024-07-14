// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Typedef.h"
#include "Engine/DataTable.h"
#include "FSFunctionLibrary.generated.h"

class USoundBase;
class UNiagaraSystem;
class UNiagaraComponent;
class AFSCharacter;

/** Various effect values attached when damaging the enemy. */
USTRUCT(BlueprintType)
struct FDamageParam : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	/** The reduction of enemy's health */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float DamageAmount = 0.0f;

	/** Forward strike fly velocity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float ForwardVelocity = 0.0f;

	/** Up strike fly velocity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float UpVelocity = 0.0f;

	/** The direction of the enemy's force */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	FRotator Direction;

	/** Can the enemy fly when he is on the ground */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	bool bEnbleFly = false;

	/** Time dilation value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float Dilation = 1.0f;

	/** The duration of time dilation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float DilTime = 0.0f;

	/** Impact effect */
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	UNiagaraSystem* ImpactVFX = nullptr;

	/** Impact sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	USoundBase* Sound = nullptr;
};

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Cause damage and collateral effects to enemies.
	 *
	 * @param DamageCauser			The Damager.
	 * @param TargetActor			The damaged actor.
	 * @param DamagedActorTArray	Actors that have been damaged.
	 * @param ImpactPoint			The location of the hit.
	 * @param DamageParam			Parameters of damage.
	 * @return Whether it was successfully executed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	static bool ApplyFuncDamage(AActor* DamageCauser, AActor* TargetActor, UPARAM(ref) TArray<AActor*>& DamagedActorTArray, const FVector ImpactPoint, const FDamageParam& DamageParam);

	/**
	 * Set the rotation of an actor.
	 * 
	 * @param Instigator	The actor to be rotated.
	 * @param bSetRot		Whether to set the rotation according to the player's input.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	static void RotateByLockState(AFSCharacter* Instigator, bool bSetRot = false);

	/** Returns default gravity scale. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float DefaultGravity() { return DEFAULT_GRAVITY; }

	/** Returns fast gravity scale. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float FastGravity() { return FAST_GRAVITY; }

	/** Returns slow gravity scale. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float SlowGravity() { return SLOW_GRAVITY; }

	/** Returns no gravity scale. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float NoGravity() { return NO_GRAVITY; }
};
