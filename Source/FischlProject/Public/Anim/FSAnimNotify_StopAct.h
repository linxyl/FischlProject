// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FSAnimNotify_StopAct.generated.h"

/**
 * Stop current action
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotify_StopAct : public UAnimNotify
{
	GENERATED_BODY()

private:
	//~ Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotify Interface.
};
