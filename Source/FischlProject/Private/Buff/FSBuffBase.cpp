// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff/FSBuffBase.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"	
#include "Engine/SkeletalMeshSocket.h"

UFSBuffBase::UFSBuffBase()
{
}

void UFSBuffBase::Init(ACharacter* Character, bool bTriggerEffect)
{
	RemainingTimes = MaxUseTimes;

	if (bTriggerEffect && Character)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			FXBegin,
			Character->GetMesh(),
			SocketName,
			FVector(),
			FRotator(),
			EAttachLocation::SnapToTargetIncludingScale,
			true
		);
	}
}