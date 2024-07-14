// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSBuffComponent.generated.h"

class UFSBuffBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSBuffComponent();

public:
	/** Add a new buff. */
	UFUNCTION(BlueprintCallable, Category = "Buff")
	void AddBuff(FName BuffName);
	
	/** Remove a buff. */
	FORCEINLINE void RemoveBuff(FName BuffName);

	/** Returns the buff by name. If not found, returns nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Buff")
	FORCEINLINE UFSBuffBase* FindBuff(FName BuffName);

	/** Returns the buff by name and cost use times. If not found, returns nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Buff")
	FORCEINLINE UFSBuffBase* FindAndUseBuff(FName BuffName);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	TSet<TSubclassOf<UFSBuffBase>> DefaultBuffs;

private:
	/** The map between name and buff */
	TMap<FName, UFSBuffBase*> MapBuff;

public:
	//~ Begin UActorComponent Interface.
	virtual void BeginPlay() override;
	//~ End UActorComponent Interface.
};
