// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSActionComponent.generated.h"

class UFSAction;
class AFSCharacter;

USTRUCT(Blueprintable)
struct FNextAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	FName ActionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	float Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	bool bHasArg;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	float Arg;
};

class UFSAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UFSAction* FindAction(FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<UFSAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(FName ActionName, AFSCharacter* Instigator, bool bHasArg = false, float Arg = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(FName ActionName, AFSCharacter* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopAction(UFSAction* Action, AFSCharacter* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopCurrentAction(AFSCharacter* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void EndBlock(AFSCharacter* Instigator);

	UPROPERTY(BlueprintReadWrite, Category = "Actions")
	bool bCanAct;

	UFSAction* CurrentAction;

protected:

	void SetCanAct(AFSCharacter* Instigator, bool b = true);

	void StartNextAction(AFSCharacter* Instigator);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UFSAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly)
	TArray<UFSAction*> ActionsArr;

	UPROPERTY(EditAnywhere, Category = "Actions")
	bool bAddAllActions;

	UPROPERTY(BlueprintReadWrite, Category = "Actions")
	FNextAction NextAction;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
