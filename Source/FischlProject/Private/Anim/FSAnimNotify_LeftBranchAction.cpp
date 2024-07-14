// Fill out your copyLeft notice in the Description page of Project Settings.


#include "Anim/FSAnimNotify_LeftBranchAction.h"
#include "Component/FSActionComponent.h"
#include "Actor/FSCharacter.h"

void UFSAnimNotify_LeftBranchAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFSCharacter* Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		Instigator->GetActionComp()->bLeftBranchFlag = true;
	}
}
