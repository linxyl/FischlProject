// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSShootComponent.generated.h"

class AFSProjectileBase;
class UNiagaraSystem;

/**
 * Used to execute shoot.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSShootComponent();

protected:
	/** All projectile classes */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AFSProjectileBase>> ProjectileClass;

	/** Socket name of the socket that spawn projectile */
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

	/** Played when shooting */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	USoundBase* Sound;

public:
	/**
	 * An interface of shooting.
	 * 
	 * @param ShootProjectileClass	Class to be shoot. If it is TSubclassOf<AFSProjectileBase>(), use default class.
	 * @param TempSerialNumber		Number of shooting. If it is -1, then use default SerialNumber.
	 */
	UFUNCTION(BlueprintCallable)
	void Shoot(TSubclassOf<AFSProjectileBase> ShootProjectileClass, int32 TempSerialNumber = -1);

	/** Select which projectile class to use. */
	FORCEINLINE void SelectProjectile(int32 Sel) { ProjectileSelect = Sel; }

public:
	/** Number of serial shoot */
	int32 SerialNumber;

	/** The interval between two shoot. */
	float Interval;

	/** Number of parallel shoot */
	int32 ParallelNumber;

	/** Angle of parallel shooting */
	float IncludedAngle;

protected:
	/** Implement of shoot. */
	UFUNCTION(BlueprintNativeEvent)
	void ExecShoot(TSubclassOf<AFSProjectileBase> ShootProjectileClass);

private:
	/** Index of projectile class */
	int32 ProjectileSelect;

	/** The number remaining to be shoot */
	int32 LeftSerialNum;

	/** Timer for serial shoot */
	FTimerHandle TimerHandle;
};
