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

public:
	/** Index of the combo action */
	UPROPERTY(BlueprintReadWrite)
	int Index;

protected:
	//~ Begin ACharacter Interface.
	virtual void StartAction_Implementation(AFSCharacter* Instigator) override;
	//~ End ACharacter Interface
};
