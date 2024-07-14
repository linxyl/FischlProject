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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Name", meta = (AllowPrivateAccess = "true"))
	FName ActionName;

	/** The time interval required to start the same action twice */
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	float Cooldown;

	/** Last instigating time */
	float LastTime;

protected:
	/** The tags that the action has */
	UPROPERTY(EditDefaultsOnly, Category = "Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ActionTags;

	/** Whether clear velocity when starting action */
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bResetVelocity : 1;

	/** Whether to update rotation when starting the action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bUpdateRotation : 1;

	/** Whether to set rotation based on player input direction */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bUseInputRotation : 1;

	/** Whether reset gravity when starting action */
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bResetGravity : 1;

	/** Whether clear the gravity when stopping action */
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bClearGravity : 1;

	/** Whether set the gravity to slow gravity value when starting action */
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	uint32 bSlowGravityAfterAction : 1;

public:
	/** Called when starting action. */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AFSCharacter* Instigator);

	/** Called when stopping action. */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AFSCharacter* Instigator);

	/** Used to handle custom events */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void CustomEvent(AFSCharacter* Instigator, AFSCharacter* Target, float Param);

	/** Returns the action name. **/
	FORCEINLINE FName GetActionName() const { return ActionName; }

	/** Returns the action tags. **/
	FORCEINLINE FGameplayTagContainer GetActionTags() const { return ActionTags; }

	FORCEINLINE float GetCooldown() const { return Cooldown; }

	FORCEINLINE float GetLastTime() const { return LastTime; }

	/** Actors that has being damaged */
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> DamagedActors;
};
