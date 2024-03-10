// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSCharacter.h"
#include "GameFramework/Character.h"
#include "Typedef.h"
#include "FSPlayerController.h"
#include "FSFischl.generated.h"

class UFSWeaponComponent;
class UFSCameraComponent;
class USphereComponent;
class UWindDirectionalSourceComponent;
class AFSProjectileBase;
class UNiagaraSystem;
class AFSPlayerController;

enum EMilitantStyle
{
	EM_None = 0,
	EM_BowFight,
	EM_Thunder,
	EM_Collaborate,
	EM_Wings
};

UCLASS()
class FISCHLPROJECT_API AFSFischl : public AFSCharacter
{
	GENERATED_BODY()

public:

	friend class AFSPlayerController;

public:
	// Sets default values for this character's properties
	AFSFischl();

	virtual bool SetOrientToMovement(bool bVal);

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* StepComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFSWeaponComponent* CompoundBowComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFSWeaponComponent* ThunderingPulseComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFSWeaponComponent* MitternachtsWaltzComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFSWeaponComponent* AlleyHunterComp;

	UPROPERTY(VisibleAnywhere)
	UFSCameraComponent* Camera;

	EMilitantStyle MilitantStyle;

public:

	void TryImplementInstruction(FInstruction Instruction);

	void StartNextInstruction();

	void OnLMouseHolding07();

	void OnRMouseHolding07();

protected:

	bool StartActionByInstruction(FInstruction Instruction);

	FInstruction NextInstruction;

	float NextInstructionTime;

	float MoveForwardVal;

	float MoveRightVal;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* FXBlackCharge;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

	void ToggleLock();

	void SwitchTarget(bool bRight);

	UFSWeaponComponent* GetWeaponByStyle(EMilitantStyle Style);

	virtual void Landed(const FHitResult& Hit);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
