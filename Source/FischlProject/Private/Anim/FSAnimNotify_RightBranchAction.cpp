// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotify_RightBranchAction.h"
#include "Component/FSActionComponent.h"
#include "Actor/FSCharacter.h"

void UFSAnimNotify_RightBranchAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFSCharacter* Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());
	if (ensure(Instigator))
	{
		Instigator->ActionComp->SetRightBranchFlag(true);
	}
}
