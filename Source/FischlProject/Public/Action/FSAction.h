// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FSFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Typedef.h"
#include "FSAction.generated.h"

class AFSCharacter;
class UFSActionComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class FISCHLPROJECT_API UFSAction : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFSAction();

public:

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer ActionTags;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	float Cooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	uint32 bSetRotation : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	uint32 bResetGravity : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	uint32 bClearGravity : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	uint32 bSlowGravityAfterAction : 1;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> DamagedActors;

	float LastTime;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AFSCharacter* Instigator, bool bHasArg = false, float Arg = 0.f);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AFSCharacter* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void CustomEvent(AFSCharacter* Instigator, AFSCharacter* Target, float Param);

protected:
};
