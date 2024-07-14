// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSActionComponent.generated.h"

class UFSAction;
class AFSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSActionComponent();

public:
	/** Return the action by name. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UFSAction* FindAction(FName ActionName);

	/** Add a new action by class. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<UFSAction> ActionClass);

	/**
	 * Start an action by name.
	 * 
	 * @param ActionName	The name of action that to be started.
	 * @param Instigator	The instigator of the action.
	 * @return whether succeed to start action.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(FName ActionName, AFSCharacter* Instigator);

	/** Stop an action by name. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(FName ActionName, AFSCharacter* Instigator);

	/** Stop an action. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopAction(UFSAction* Action, AFSCharacter* Instigator);

	/** Stop the current action. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopCurrentAction(AFSCharacter* Instigator);

	/** Get the current action. */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Actions")
	FORCEINLINE UFSAction* GetCurrentAction() { return CurrentAction; }

	/** Stop blocking action so that other actions can be started. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void EndBlock(AFSCharacter* Instigator);

protected:
	/** Actions owned default */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UFSAction>> DefaultActions;

	/** All actions owned current */
	UPROPERTY(BlueprintReadOnly)
	TArray<UFSAction*> ActionsArr;

	/** Whether add all actions in DefaultActions to ActionArr */
	UPROPERTY(EditAnywhere, Category = "Actions")
	bool bAddAllActions;

	/** Whether can start act and move */
	UPROPERTY(BlueprintReadWrite, Category = "Actions")
	bool bCanAct;

public:
	/** Whether the left branch action can be started. */
	bool bLeftBranchFlag;
	/** Whether the right branch action can be started. */
	bool bRightBranchFlag;

private:
	UFSAction* CurrentAction;

	/** Set the character can act and move. */
	void SetCanAct(AFSCharacter* Instigator, bool b = true);

public:
	//~ Begin UActorComponent Interface.
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.
};
