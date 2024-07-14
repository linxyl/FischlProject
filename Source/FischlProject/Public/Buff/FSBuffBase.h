// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FSBuffBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FISCHLPROJECT_API UFSBuffBase : public UObject
{
	GENERATED_BODY()

public:
	UFSBuffBase();

public:
	/** Should be called when being Created. */
	void Init(ACharacter* Character, bool bTriggerEffect = false);

protected:
	/** Tags that owned by the buff */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BuffTags;

	/** Maximum usable times. If it is -1, it can be unlimited used. */
	UPROPERTY(EditDefaultsOnly)
	int32 MaxUseTimes = -1;

	/** Effect that triggered when being created */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UNiagaraSystem* FXBegin;

	/** Name of the socket where spawn effect */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FName SocketName;

private:
	/** The remaining number of uses */
	int32 RemainingTimes;

	friend class UFSBuffComponent;
};
