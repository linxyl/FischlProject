// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSCharacter.h"
#include "FSOz.generated.h"

class AFSFischl;

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API AFSOz : public AFSCharacter
{
	GENERATED_BODY()

	friend AFSFischl;

public:
	/** Default UObject constructor. */
	AFSOz();

private:
	/** Owner of character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFSFischl* Fischl;

	/** effect in stealth. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* StealthVFX;

	/** Component of StealthVFX. */
	UNiagaraComponent* StealthVFXComp;

public:
	/** Let Oz visible. */
	UFUNCTION(BlueprintCallable)
	void Appear();

	/** Hide Oz. */
	UFUNCTION(BlueprintCallable)
	void Hide();

	/** Should be call after action. */
	UFUNCTION(BlueprintCallable)
	void PostAction();

	/** Returns Fischl **/
	FORCEINLINE AFSFischl* GetFischl() const { return Fischl; }

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

private:
	/** choose whether to display effect according to current character state. */
	void DisplayFX();

	/** Calculate the target location in stealth. */
	FVector GetTargetStealthLocation() const;
};
