// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotify_Shoot.h"

#include "Actor/FSCharacter.h"
#include "Component/FSShootComponent.h"

void UFSAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFSCharacter* Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		Instigator->GetShootComp()->Shoot(ProjectileClass, SerialNumber);
	}
}
