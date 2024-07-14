// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "Containers/Map.h"
#include "FSAnimNotifyState_FlyMode.generated.h"

/**
 * Set to fly mode in the window
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_FlyMode : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

protected:
	/** Whether to stop moving at the beginning */
	UPROPERTY(EditAnywhere)
	bool bStopMove;

private:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.
};
