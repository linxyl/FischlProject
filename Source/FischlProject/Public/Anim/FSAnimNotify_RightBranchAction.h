// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FSAnimNotify_RightBranchAction.generated.h"

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotify_RightBranchAction : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
