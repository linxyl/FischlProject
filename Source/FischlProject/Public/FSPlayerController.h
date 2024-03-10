// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Typedef.h"

#include "FSPlayerController.generated.h"

enum EInstruction_Key
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

enum EInstruction_Seq
{
	EI_BackForward = 1,
	EI_ForwardBack
};

enum EInstruction_Tail
{
	EI_LMouse_Press = 1,
	EI_LMouse_Holding07,
	EI_RMouse_Press,
	EI_RMouse_Holding07,
	EI_F_Press,
	EI_R_Press,
	EI_Space_Press,
	EI_Shift_Press,
};

struct FInstructionSeq
{
public:
	EInstruction_Key InsKey[MAX_INS_NUM];
	float InstigateTime[MAX_INS_NUM];

	int32 Pos;

public:

	void Push(EInstruction_Key Key, float Time)
	{
		this->InsKey[Pos] = Key;
		this->InstigateTime[Pos] = Time;

		if (++Pos >= MAX_INS_NUM)
		{
			Pos = 0;
		}
	}

	EInstruction_Key LookBackValidIns(float CurTime, int32 Offset = 1)
	{
		if (Offset < 1 || Offset >= MAX_INS_NUM)
			return (EInstruction_Key)0;

		int t = Pos - Offset;
		if (t < 0)
		{
			t += MAX_INS_NUM;
		}

		if (CurTime - InstigateTime[t] <= Offset * VALID_INS_TIME)
		{
			return InsKey[t];
		}
		return (EInstruction_Key)0;
	}

	EInstruction_Seq GetSeq(float WorldTime)
	{
		if (EI_Back == LookBackValidIns(WorldTime, 2) &&
			EI_Forward == LookBackValidIns(WorldTime))
			return EI_BackForward;
		if (EI_Forward == LookBackValidIns(WorldTime, 2) &&
			EI_Back == LookBackValidIns(WorldTime))
			return EI_ForwardBack;
		return (EInstruction_Seq)0;
	}
};

struct FInstruction
{
	EInstruction_Seq Seq;
	EInstruction_Hold Hold;
	EInstruction_Tail Tail;
	float Arg = 0.f;
};

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API AFSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AFSPlayerController();

protected:

	class AFSFischl* PlayerCharacter;

	FInstructionSeq InsSeq;

	EInstruction_Key GetInsKey(const FRotator& Rot);

	EInstruction_Hold GetInsHold();

	float GetInsDir();

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void PlayerTick(float DeltaTime) override;

protected:

	float LMouseHoldingTime;

	float RMouseHoldingTime;

protected:

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
};
