// Fill out your copyLeft notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FSAnimNotify_LeftBranchAction.generated.h"

/**
 * If notified, you can use the left branch action
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotify_LeftBranchAction : public UAnimNotify
{
	GENERATED_BODY()

private:
	//~ Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotify Interface.
};
