// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "Projectile/FSProjectileBase.h"

#include "FSAnimNotify_Shoot.generated.h"

/**
 * Shoot projectile
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotify_Shoot : public UAnimNotify
{
	GENERATED_BODY()

protected:
	/** Class of projectiles. If it is empty, use default value */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFSProjectileBase> ProjectileClass;

	/** Number of shoot serial times. If it is -1, use default value */
	UPROPERTY(EditAnywhere)
	int32 SerialNumber = -1;

private:
	//~ Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotify Interface.
};
