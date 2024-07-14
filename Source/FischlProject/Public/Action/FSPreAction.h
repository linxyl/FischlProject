// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff/FSBuffBase.h"
#include "Action/FSAction.h"
#include "FSPreAction.generated.h"


class FSBuffBase;

/**
 * Action that used to add buff
 */
UCLASS()
class FISCHLPROJECT_API UFSPreAction : public UFSAction
{
	GENERATED_BODY()

protected:
	/** The class of buff that would to be added */
	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	TSubclassOf<UFSBuffBase> BuffClass;

	/** Effect that spawned when adding buff */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* FX;

private:
	UFSBuffBase* Buff;

protected:
	//~ Begin ACharacter Interface.
	virtual void StartAction_Implementation(AFSCharacter* Instigator) override;
	//~ End ACharacter Interface
};
