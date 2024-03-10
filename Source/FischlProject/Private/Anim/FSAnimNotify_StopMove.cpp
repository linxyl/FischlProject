// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotify_StopMove.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSAnimNotify_StopMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter* Instigator = Cast<ACharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		Instigator->GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
	}
}
