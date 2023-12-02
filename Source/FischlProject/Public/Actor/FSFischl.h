// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSCharacter.h"
#include "GameFramework/Character.h"
#include "Typedef.h"
#include "FSFischl.generated.h"

enum EMilitantStyle
{
	EM_None = 0,
	EM_BowFight,
	EM_Thunder,
	EM_Collaborate,
	EM_Wings
};

enum EInstruction_Head
{
	EI_Forward = 1,
	EI_Back,
	EI_Right,
	EI_Left
};

enum EInstruction_Hold
{
	EI_None = 0,
	EI_Forward_Hold = 1,
	EI_Back_Hold,
	EI_Right_Hold,
	EI_Left_Hold
};

enum EInstruction_Tail
{
	EI_LMouse_Press = 1,
	EI_LMouse_Release,
	EI_RMouse_Press,
	EI_RMouse_Release,
	EI_F_Press,
	EI_F_Release,
	EI_R_Press,
	EI_R_Release,
	EI_Space_Press,
	EI_Space_Release,
	EI_Shift_Press,
	EI_Shift_Release
};

class UFSWeaponComponent;
class UFSCameraComponent;
class USphereComponent;
class UWindDirectionalSourceComponent;
class AFSProjectileBase;
class UNiagaraSystem;

UCLASS()
class FISCHLPROJECT_API AFSFischl : public AFSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSFischl();

	virtual bool SetOrientToMovement(bool bVal);

	UFUNCTION(BlueprintCallable)
	void ShootArrow();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* StepComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting;

protected:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AFSProjectileBase> ProjectileClass;

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

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* ShootVFX;

	float MoveForwardVal;

	float MoveRightVal;

	EMilitantStyle MilitantStyle;

	struct InstructionSeq
	{
		EInstruction_Head Instruction[MAX_INS_NUM];

		float InstigateTime[MAX_INS_NUM];

		int32 Pos;

		EInstruction_Head LookBackValidIns(float CurTime, int32 Offset = 1)
		{
			if (Offset < 1 || Offset >= MAX_INS_NUM)
				return (EInstruction_Head)0;

			int t = Pos - Offset;
			if (t < 0)
			{
				t += MAX_INS_NUM;
			}

			if (CurTime - InstigateTime[t] <= Offset * VALID_INS_TIME)
			{
				return Instruction[t];
			}
			return (EInstruction_Head)0;
		}
	} InsSeq;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookYaw(float Value);

	void LookPitch(float Value);

	void W_Press();

	void S_Press();

	void A_Press();

	void D_Press();

	void F_Press();

	void R_Press();

	void Q_Press();

	void E_Press();

	void Shift_Press();

	void Shift_Release();

	void Space_Press();

	void Left_Press();

	void Left_Release();

	void Right_Press();

	void Right_Release();

	void Middle_Press();

	void Middle_Release();

	void Key1_Press();

	void Key2_Press();

	void Key3_Press();

	void Key4_Press();

	void ScrollUp();

	void ScrollDown();

	void ToggleLock();

	void SwitchTarget(bool bRight);

	UFSWeaponComponent* GetWeaponByStyle(EMilitantStyle Style);

	EInstruction_Head GetInsHead(FRotator& Rot);

	EInstruction_Hold GetInsHold();

	float GetInsDir();

	bool StartActionByInstruction(InstructionSeq& Seq, EInstruction_Hold Hold, EInstruction_Tail Tail, float Arg = 0.f);

	virtual void Landed(const FHitResult& Hit);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
