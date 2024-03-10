// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSShootComponent.generated.h"

class AFSProjectileBase;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSShootComponent();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFSProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ShootVFX;

public:
	UFUNCTION(BlueprintCallable)
	void Shoot();

public:
	uint32 SerialNumber;

	uint32 ParallelNumber;

	float Interval;

	float IncludedAngle;

protected:
	void OnceShoot();

	uint32 LeftSerialNum;

	FTimerHandle TimerHandle;
};
