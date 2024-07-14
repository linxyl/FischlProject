// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/FSAction_ComboBase.h"
#include "Actor/FSCharacter.h"

UFSAction_ComboBase::UFSAction_ComboBase()
{
	ActionTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combo"));

	Index = 0;
}

void UFSAction_ComboBase::StartAction_Implementation(AFSCharacter* Instigator)
{
	Super::StartAction_Implementation(Instigator);
}
