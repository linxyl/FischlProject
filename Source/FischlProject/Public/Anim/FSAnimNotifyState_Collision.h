// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "FSFunctionLibrary.h"
#include "FSAnimNotifyState_Collision.generated.h"

class AFSCharacter;
class UFSAction;
class UDataTable;

/**
 * Used for collision detection
 */
UCLASS()
class FISCHLPROJECT_API UFSAnimNotifyState_Collision : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	UFSAnimNotifyState_Collision();

protected:
	/** Socket name of the collision detection location */
	UPROPERTY(EditAnywhere)
	FName Socket;

	/** The start position offset of capsule collision */
	UPROPERTY(EditAnywhere)
	FVector OffsetStart;

	/** The end position offset of capsule collision */
	UPROPERTY(EditAnywhere)
	FVector OffsetEnd;

	/** The radius of the capsule collision */
	UPROPERTY(EditAnywhere)
	float CollisionRadius;

	/** The owner action name */
	UPROPERTY(EditAnywhere)
	FName ActionName;

	/** The profile name of the collision */
	UPROPERTY(EditAnywhere)
	FName CollisionProfile;

	/** Whether to detect only one collision and end */
	UPROPERTY(EditAnywhere)
	bool bOnce;

	/** Table of damage parameters to reference for colliding actor */
	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;

	/** Row name of the variable DataTable */
	UPROPERTY(EditAnywhere)
	FName RowName;

private:
	AFSCharacter* Instigator;

	UFSAction* Action;

	bool OnceFlag;

	FDamageParam* DamageParam;

private:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.
};
