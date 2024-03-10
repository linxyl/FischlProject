// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "FSAnimNotifyState_Move.generated.h"

class AFSCharacter;

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_Move : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	UFSAnimNotifyState_Move();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	float ForwardVelocity;

	UPROPERTY(EditAnywhere)
	float UpVelocity;

	AFSCharacter* Instigator;
};
