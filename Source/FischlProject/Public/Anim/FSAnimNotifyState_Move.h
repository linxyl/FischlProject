// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "FSAnimNotifyState_Move.generated.h"

class AFSCharacter;

/**
 * Keep the character moving
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_Move : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	UFSAnimNotifyState_Move();

protected:
	/** Horizontal component velocity */
	UPROPERTY(EditAnywhere)
	float ForwardVelocity;

	/** Vertical component velocity */
	UPROPERTY(EditAnywhere)
	float UpVelocity;

	/** Whether clear velocity when end */
	UPROPERTY(EditAnywhere)
	bool bStopMoveWhenEnd;

	/** Moving character */
	AFSCharacter* Instigator;

private:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	//~ End UAnimNotifyState Interface.
};
