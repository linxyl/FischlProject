// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/FSAction.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Actor/FSCharacter.h"
#include "FSFunctionLibrary.h"

UFSAction::UFSAction()
{
	Cooldown = 0.f;
	LastTime = 0.f;

	//bSetRotation = false;
	bUpdateRotation = true;
	bUseInputRotation = false;

	bSlowGravityAfterAction = 1;
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSAction::StartAction_Implementation(AFSCharacter* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));

	LastTime = GetWorld()->TimeSeconds;

	DamagedActors.Empty();

	if (bResetVelocity)
	{
		Instigator->GetCharacterMovement()->Velocity = { 0.f, 0.f, 0.f };
	}

	if (!ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Release")) && bUpdateRotation)
	{
		UFSFunctionLibrary::RotateByLockState(Instigator, bUseInputRotation);
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

	//Instigator->bIsLaunching = false;

	if (bSlowGravityAfterAction)
	{
		Instigator->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::SlowGravity();
	}
}

void UFSAction::CustomEvent_Implementation(AFSCharacter* Instigator, AFSCharacter* Target, float Param)
{
}
