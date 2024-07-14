// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Typedef.h"
#include "Actor/FSCharacter.h"
#include "FSPlayerController.h"
#include "FSFischl.generated.h"

class UFSWeaponComponent;
class UFSCameraComponent;
class USphereComponent;
class UWindDirectionalSourceComponent;
class AFSProjectileBase;
class UNiagaraSystem;
class AFSPlayerController;
class AFSOz;

enum EMilitantStyle
{
	MilitantStyle_None = 0,
	MilitantStyle_BowFight,
	MilitantStyle_Thunder,
	MilitantStyle_Collaborate,
	MilitantStyle_Wings
};

UCLASS()
class FISCHLPROJECT_API AFSFischl : public AFSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSFischl();

private:
	/** Component that managing camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSCameraComponent* CameraComp;

	//////////////////////////////////////////////////////////////////////////
	// Weapons components
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere)
	UFSWeaponComponent* CompoundBowComp;

	UPROPERTY(VisibleAnywhere)
	UFSWeaponComponent* ThunderingPulseComp;

	UPROPERTY(VisibleAnywhere)
	UFSWeaponComponent* MitternachtsWaltzComp;

	UPROPERTY(VisibleAnywhere)
	UFSWeaponComponent* AlleyHunterComp;

private:

	//////////////////////////////////////////////////////////////////////////
	//  Begin Oz character relative
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFSOz* Oz;

	UPROPERTY(EditDefaultsOnly, Category = "Oz")
	TSubclassOf<AFSOz> OzClass;

public:
	/** Returns Oz **/
	FORCEINLINE AFSOz* GetOz() const { return Oz; }

	//////////////////////////////////////////////////////////////////////////
	//  End Oz character relative
	//////////////////////////////////////////////////////////////////////////

public:

	//////////////////////////////////////////////////////////////////////////
	// Instruction used to start action.
	//////////////////////////////////////////////////////////////////////////

	/** Try to execute the function. If failed, Use this instruction as the next instruction. */
	void TryImplementInstruction(FInstruction Instruction);

	/** Start the last unfinished instruction. */
	void StartNextInstruction();

	/** Called when left mouse long press. */
	void OnLMouseHolding();
	/** Called when right mouse long press. */
	void OnRMouseHolding07();

private:

	//////////////////////////////////////////////////////////////////////////
	// Start action by parsing instruction.
	//////////////////////////////////////////////////////////////////////////

	/** select an action to start by parsing instruction. */
	bool StartActionByInstruction(FInstruction Instruction);

	/** Start action of BowFight Militant Style. */
	bool StartBowFightAction(FInstruction Instruction);
	/** Start action of Thunder Militant Style. */
	bool StartThunderAction(FInstruction Instruction);
	/** Start action of Collaborate Militant Style. */
	bool StartCollaborateAction(FInstruction Instruction);
	/** Start action of Wings Militant Style. */
	bool StartWingsAction(FInstruction Instruction);

	/** The instruction to be started next. */
	FInstruction NextInstruction;

	/** The time of start instruction. */
	float NextInstructionTime;

private:
	/** Current militant style. */
	EMilitantStyle MilitantStyle;

	void ChangeStyle(EMilitantStyle Style);

	/** Get the relative weapon by militant style. */
	UFSWeaponComponent* GetWeaponByStyle(EMilitantStyle Style);

	friend class AFSPlayerController;

	/** Toggle between locking and unlocking. */
	void ToggleLock();

	/**
	 * Switch locked actor.
	 * @param	bRight	whether Search from the right.
	 */
	void SwitchTarget(bool bRight);

private:
	/** Store the value of player pressing forward or back. */
	float MoveForwardVal;
	/** Store the value of player pressing right or left. */
	float MoveRightVal;

public:
	/** Returns whether player is inputing the movement key. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsInputingMoving() { return MoveForwardVal || MoveRightVal; }

	/** Returns the relative direction of player inputing movement. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetInputMovingDirection()
	{
		return FVector(MoveForwardVal, MoveRightVal, 0.0f).Rotation().Yaw;
	}

	/** Get overlapped actors to decide whether to start step action */
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* StepComp;

	/** Whether to enable double jump. */
	UPROPERTY(BlueprintReadWrite)
	bool bEnDoubleJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting;

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

	//~ Begin ACharacter Interface.
	virtual void Landed(const FHitResult& Hit) override;
	//~ End ACharacter Interface

	//~ Begin AFSCharacter Interface.
	virtual bool UpdateOrientToMovement(bool bVal) override;
	//~ End AFSCharacter Interface

private:
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* FXBlackCharge;
};
