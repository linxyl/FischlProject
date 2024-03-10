// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotify_StopAct.h"
#include "Actor/FSCharacter.h"
#include "Component/FSActionComponent.h"

void UFSAnimNotify_StopAct::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFSCharacter* Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		Instigator->ActionComp->StopCurrentAction(Instigator);
	}
}
