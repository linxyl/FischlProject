// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/FSAction.h"
#include "FSAction_ComboBase.generated.h"

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSAction_ComboBase : public UFSAction
{
	GENERATED_BODY()

public:

	UFSAction_ComboBase();

	virtual void StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg = false, float Arg = 0.f);

	UPROPERTY(BlueprintReadWrite)
	int Index;
};
