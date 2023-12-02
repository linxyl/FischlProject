// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/FSAction.h"
#include "FSAction_ArrowFan.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FISCHLPROJECT_API UFSAction_ArrowFan : public UFSAction
{
	GENERATED_BODY()

public:

	UFSAction_ArrowFan();

	virtual void StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg = false, float Arg = 0.f) override;
};
