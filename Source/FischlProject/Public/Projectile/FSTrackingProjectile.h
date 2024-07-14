// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/FSProjectileBase.h"
#include "FSTrackingProjectile.generated.h"

/**
 * Projectile that can tracking an enemy;
 */
UCLASS()
class FISCHLPROJECT_API AFSTrackingProjectile : public AFSProjectileBase
{
	GENERATED_BODY()
	
public:
	AFSTrackingProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float MaxAngularVelocity;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface
};
