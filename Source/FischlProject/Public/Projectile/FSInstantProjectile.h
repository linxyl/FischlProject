// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/FSProjectileBase.h"
#include "FSInstantProjectile.generated.h"

/**
 * No moving process, and explodes directly in the target actor location.
 */
UCLASS()
class FISCHLPROJECT_API AFSInstantProjectile : public AFSProjectileBase
{
	GENERATED_BODY()
	
public:
	AFSInstantProjectile();

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
