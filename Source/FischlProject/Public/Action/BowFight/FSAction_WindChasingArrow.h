// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/FSAction.h"
#include "FSAction_WindChasingArrow.generated.h"

class AFSProjectileBase;

/**
 * 
 */
UCLASS(Blueprintable)
class FISCHLPROJECT_API UFSAction_WindChasingArrow : public UFSAction
{
	GENERATED_BODY()

public:

	UFSAction_WindChasingArrow();

	virtual void StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg = false, float Arg = 0.f) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AFSProjectileBase> ProjectileClass;
};
