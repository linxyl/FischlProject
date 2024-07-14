// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Typedef.h"
#include "FSPlayerController.generated.h"

/** Used for sequence instruction */
enum EInstructionKey
{
	InstructionKey_None = 0,
	InstructionKey_Forward,
	InstructionKey_Back,
	InstructionKey_Right,
	InstructionKey_Left
};

/** The key being holding */
enum EInstructionHold
{
	InstructionHold_None = 0,
	InstructionHold_Forward,
	InstructionHold_Back,
	InstructionHold_Right,
	InstructionHold_Left
};

/** Sequence instruction composed of multiple operating */
enum EInstructionSeq
{
	InstructionSeq_None = 0,
	InstructionSeq_BackForward,
	InstructionSeq_ForwardBack
};

/** The last action of operating */
enum EInstructionTail
{
	InstructionTail_None = 0,
	InstructionTail_LeftMouse_Press,
	InstructionTail_LeftMouse_Release,
	InstructionTail_RightMouse_Press,
	InstructionTail_RightMouse_Holding07,
	InstructionTail_F_Press,
	InstructionTail_R_Press,
	InstructionTail_Space_Press,
	InstructionTail_Shift_Press,
};

/** Used to record the pressed key to get the sequence instruction */
class FInstructionSeq
{
private:
	/** Used to record the pressed key */
	EInstructionKey InsKey[MAX_INS_NUM];

	/** Used to record the time of pressing */
	float InstigateTime[MAX_INS_NUM];

	/** Current InsKey position */
	int32 Pos;

public:
	/** Record the info of the last pressing. */
	void Push(EInstructionKey Key, float Time)
	{
		this->InsKey[Pos] = Key;
		this->InstigateTime[Pos] = Time;

		if (++Pos >= MAX_INS_NUM)
		{
			Pos = 0;
		}
	}

	/** Get sequence instruction. */
	EInstructionSeq GetSeq(float WorldTime)
	{
		if (InstructionKey_Back == LookBackValidKey(WorldTime, 2) &&
			InstructionKey_Forward == LookBackValidKey(WorldTime))
			return InstructionSeq_BackForward;
		if (InstructionKey_Forward == LookBackValidKey(WorldTime, 2) &&
			InstructionKey_Back == LookBackValidKey(WorldTime))
			return InstructionSeq_ForwardBack;
		return InstructionSeq_None;
	}

private:
	/** Gets the key that has been pressed in the past. */
	EInstructionKey LookBackValidKey(float CurTime, int32 Offset = 1)
	{
		if (Offset < 1 || Offset >= MAX_INS_NUM)
			return InstructionKey_None;

		int t = Pos - Offset;
		if (t < 0)
		{
			t += MAX_INS_NUM;
		}

		if (CurTime - InstigateTime[t] <= Offset * VALID_INS_TIME)
		{
			return InsKey[t];
		}
		return InstructionKey_None;
	}
};

/** Package of instruction */
struct FInstruction
{
	EInstructionSeq Seq;
	EInstructionHold Hold;
	EInstructionTail Tail;
};

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API AFSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Default Constructor */
	AFSPlayerController();

private:
	/** Owner of this */
	class AFSFischl* PlayerCharacter;

	FInstructionSeq InsSeq;

	/** Get key instruction. */
	EInstructionKey GetInsKey(const FRotator& Rot);

	/** Get hold instruction. */
	EInstructionHold GetInsHold();

private:
	/** Left mouse button holding time. */
	float LeftMouseHoldingTime;
	/** Right mouse button holding time. */
	float RightMouseHoldingTime;

private:
	/** Processing values in front and back directions. */
	void MoveForward(float Value);
	/** Processing values in right and left directions. */
	void MoveRight(float Value);
	/** Processing the left and right movement of the mouse. */
	void LookYaw(float Value);
	/** Processing the up and down movement of the mouse. */
	void LookPitch(float Value);

	/** Called when W is pressed. */
	void W_Press();
	/** Called when S is pressed. */
	void S_Press();
	/** Called when A is pressed. */
	void A_Press();
	/** Called when D is pressed. */
	void D_Press();
	/** Called when F is pressed. */
	void F_Press();
	/** Called when R is pressed. */
	void R_Press();
	/** Called when Q is pressed. */
	void Q_Press();
	/** Called when E is pressed. */
	void E_Press();
	/** Called when Shift is pressed. */
	void Shift_Press();
	/** Called when Shift is released. */
	void Shift_Release();
	/** Called when Space is pressed. */
	void Space_Press();
	/** Called when left mouse is pressed. */
	void Left_Press();
	/** Called when left mouse is released. */
	void Left_Release();
	/** Called when right mouse is pressed. */
	void Right_Press();
	/** Called when right mouse is released. */
	void Right_Release();
	/** Called when middle mouse is pressed. */
	void Middle_Press();
	/** Called when middle mouse is released. */
	void Middle_Release();
	/** Called when key 1 is pressed. */
	void Key1_Press();
	/** Called when key 2 is pressed. */
	void Key2_Press();
	/** Called when key 3 is pressed. */
	void Key3_Press();
	/** Called when key 4 is pressed. */
	void Key4_Press();
	/** Called when mouse wheel up. */
	void ScrollUp();
	/** Called when mouse wheel down. */
	void ScrollDown();

protected:
	//~ Begin AController Interface
	virtual void SetPawn(APawn* InPawn) override;
	//~ End AController Interface

	//~ Begin APlayerController Interface.
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface
};
