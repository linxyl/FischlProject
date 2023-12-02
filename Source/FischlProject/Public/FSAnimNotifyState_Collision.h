// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "FSAnimNotifyState_Collision.generated.h"

class AFSCharacter;
class UFSAction;

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_Collision : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	UFSAnimNotifyState_Collision();

	UPROPERTY(EditAnywhere)
	FName SocketStart;

	UPROPERTY(EditAnywhere)
	FName SocketEnd;

	UPROPERTY(EditAnywhere)
	FName ActionName;

	UPROPERTY(EditAnywhere)
	FName CollisionProfile;

	UPROPERTY(EditAnywhere)
	float CollisionRadius;

	UPROPERTY(EditAnywhere)
	bool bOnce;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

protected:
	AFSCharacter* Instigator;

	UFSAction* Action;

	TArray<FHitResult> AllResults;

	bool OnceFlag;
};
