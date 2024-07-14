// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/FSSummonAction.h"

UFSSummonAction::UFSSummonAction()
{
	bUpdateRotation = false;

	bSlowGravityAfterAction = 0;

	ActionTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Action.Release")));
}
