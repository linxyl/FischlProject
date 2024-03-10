// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotifyState_FlyMode.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "FSFunctionLibrary.h"

static TMap<AActor*, int> MapFlyMode;

void UFSAnimNotifyState_FlyMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter* Instigator = Cast<ACharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		if (bStopMove)
		{
			Instigator->GetCharacterMovement()->Velocity = { 0.f,0.f,0.f };
		}

		if (MapFlyMode.Contains(Instigator))
			++MapFlyMode[Instigator];
		else
			MapFlyMode.Add(Instigator, 1);
		Instigator->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Instigator->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::NoGravity();
	}
}

void UFSAnimNotifyState_FlyMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter* Instigator = Cast<ACharacter>(MeshComp->GetOwner());
	if (Instigator)
	{
		if (--MapFlyMode[Instigator] == 0)
		{
			Instigator->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			MapFlyMode.Remove(Instigator);
		}
	}
}
