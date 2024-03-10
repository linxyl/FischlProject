// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/FSAction.h"
#include "Actor/FSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FSFunctionLibrary.h"

UFSAction::UFSAction()
{
	Cooldown = 0.f;
	LastTime = 0.f;

	bSetRotation = 1;

	bSlowGravityAfterAction = 1;
	PrimaryComponentTick.bCanEverTick = false;
}

void UFSAction::StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg, float Arg)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s, bHasArg = %d, Arg = %f"), *GetNameSafe(this), bHasArg, Arg);

	LastTime = GetWorld()->TimeSeconds;

	DamagedActors.Empty();

	if (bSetRotation)
	{
		if (Instigator->bIsLocked)
		{
			Instigator->SetActorRotation(FRotator(0.f, Instigator->LockedRot.Yaw, 0.f));
		}
		else if (bHasArg)
		{
			Instigator->SetActorRotation(FRotator(0.f, Arg, 0.f));
		}
	}

	if (bResetGravity)
	{
		Instigator->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::DefaultGravity();
	}
	else if (bClearGravity)
	{
		Instigator->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::NoGravity();
	}
}

void UFSAction::StopAction_Implementation(AFSCharacter* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	Instigator->bIsLaunching = false;

	if (bSlowGravityAfterAction)
	{
		Instigator->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::SlowGravity();
	}
}

void UFSAction::CustomEvent_Implementation(AFSCharacter* Instigator, AFSCharacter* Target, float Param)
{

}
