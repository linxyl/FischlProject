// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "FSAnimNotifyState_DisableRootMotion.generated.h"

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_DisableRootMotion : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
