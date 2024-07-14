// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FSGameModeBase.generated.h"


class UFSBuffBase;
class UFSBuffComponent;

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API AFSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Get Buff by FName. */
	FORCEINLINE TSubclassOf<UFSBuffBase> FindBuffClass(FName BuffName);

private:
	/** Set of Buff */
	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	TSet<TSubclassOf<UFSBuffBase>> BuffClasses;

	/** Map of FName and Buff */
	TMap<FName, TSubclassOf<UFSBuffBase>> BuffClassMap;

protected:
	//~ Begin AGameModeBase Interface.
	virtual void StartPlay() override;
	//~ End AGameModeBase Interface
};
