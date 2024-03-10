// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/CharacterMovementComponent.h"

#include "Component/FSActionComponent.h"
#include "Action/FSAction_ComboBase.h"
#include "Actor/FSCharacter.h"
#include "Actor/FSFischl.h"

// Sets default values for this component's properties
UFSActionComponent::UFSActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bCanAct = true;

	bAddAllActions = true;

	CurrentAction = nullptr;

	//NextAction.ActionName = "";
	//NextAction.Time = 0.f;
}

UFSAction* UFSActionComponent::FindAction(FName ActionName)
{
	for (auto& Action : ActionsArr)
	{
		if (Action->ActionName == ActionName)
		{
			return Action;
		}
	}
	return nullptr;
}

void UFSActionComponent::AddAction(TSubclassOf<UFSAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UFSAction* NewAction = NewObject<UFSAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		ActionsArr.Add(NewAction);
	}
}

bool UFSActionComponent::StartActionByName(FName ActionName, AFSCharacter* Instigator, bool bHasArg, float Arg)
{
	for (UFSAction* Action : ActionsArr)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (GetWorld()->TimeSeconds - Action->LastTime < Action->Cooldown)
			{
				return false;
			}
			
			if (Action->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Release")))
			{
				Action->StartAction(Instigator, bHasArg, Arg);
			}
			else if (bCanAct || (CurrentAction->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Interrupted")) && CurrentAction != Action))
			{
				if (Action->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Combo")))
				{
					if (CurrentAction == Action)
					{
						++Cast<UFSAction_ComboBase>(Action)->Index;
					}
					else
					{
						Cast<UFSAction_ComboBase>(Action)->Index = 0;
					}
				}

				StopCurrentAction(Instigator);
				CurrentAction = Action;
				SetCanAct(Instigator, false);

				Action->StartAction(Instigator, bHasArg, Arg);
			}
			else if (Action->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Force")))
			{
				StopCurrentAction(Instigator);
				CurrentAction = Action;
				SetCanAct(Instigator, false);

				Action->StartAction(Instigator, bHasArg, Arg);
			}
			else
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool UFSActionComponent::StopActionByName(FName ActionName, AFSCharacter* Instigator)
{
	return true;
}

bool UFSActionComponent::StopAction(UFSAction* Action, AFSCharacter* Instigator)
{
	Action->StopAction(Instigator);

	return true;
}

bool UFSActionComponent::StopCurrentAction(AFSCharacter* Instigator)
{
	if (nullptr == CurrentAction)
		return true;

	StopAction(CurrentAction, Instigator);
	CurrentAction = nullptr;
	SetCanAct(Instigator, true);
	SetLeftBranchFlag(false);
	SetRightBranchFlag(false);

	return true;
}

void UFSActionComponent::EndBlock(AFSCharacter* Instigator)
{
	SetCanAct(Instigator, true);
	AFSFischl* Fischl = Cast<AFSFischl>(Instigator);
	if (Fischl)
		Fischl->StartNextInstruction();
	//StartNextAction(Instigator);
}

void UFSActionComponent::SetCanAct(AFSCharacter* Instigator, bool b /*= true*/)
{
	bCanAct = b;

	Instigator->SetOrientToMovement(b);
}

//void UFSActionComponent::StartNextAction(AFSCharacter* Instigator)
//{
//	if (GetWorld()->TimeSeconds - NextAction.Time <= 0.4)
//		StartActionByName(NextAction.ActionName, Instigator, NextAction.bHasArg, NextAction.Arg);
//	NextAction.Time = 0.f;
//}

// Called when the game starts
void UFSActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (bAddAllActions) {
		for (TSubclassOf<UFSAction> ActionClass : DefaultActions)
		{
			AddAction(ActionClass);
		}
	}
}


// Called every frame
void UFSActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

