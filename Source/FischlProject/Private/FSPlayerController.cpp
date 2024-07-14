// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Actor/FSFischl.h"
#include "Component/FSActionComponent.h"
#include "Action/FSAction.h"
#include "Component/FSCameraComponent.h"
#include "Component/FSWeaponComponent.h"

AFSPlayerController::AFSPlayerController():
	Super(),
	LeftMouseHoldingTime(-1.f),
	RightMouseHoldingTime(-1.f)
{
}

EInstructionKey AFSPlayerController::GetInsKey(const FRotator& Rot)
{
	if (PlayerCharacter->LockedActor)
	{
		PlayerCharacter->LockedRot = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), PlayerCharacter->LockedActor->GetActorLocation());
	}
	FRotator DelRot = UKismetMathLibrary::NormalizedDeltaRotator(PlayerCharacter->LockedRot, Rot);

	if (-45.0f <= DelRot.Yaw && DelRot.Yaw <= 45.0f)
	{
		return InstructionKey_Forward;
	}
	else if (45.0f < DelRot.Yaw && DelRot.Yaw < 135.0f)
	{
		return InstructionKey_Left;
	}
	else if (-45.0f > DelRot.Yaw && DelRot.Yaw > -135.0f)
	{
		return InstructionKey_Right;
	}
	else
	{
		return InstructionKey_Back;
	}
}

EInstructionHold AFSPlayerController::GetInsHold()
{
	if (!PlayerCharacter->bIsLocked || (abs(PlayerCharacter->MoveForwardVal) <= KINDA_SMALL_NUMBER && abs(PlayerCharacter->MoveRightVal) <= KINDA_SMALL_NUMBER))
	{
		return InstructionHold_None;
	}

	if (PlayerCharacter->LockedActor)
	{
		PlayerCharacter->LockedRot = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), PlayerCharacter->LockedActor->GetActorLocation());
	}

	FRotator TargetRot = GetControlRotation();
	TargetRot.Yaw += FVector(PlayerCharacter->MoveForwardVal, PlayerCharacter->MoveRightVal, 0.0f).Rotation().Yaw;

	FRotator DelRot = UKismetMathLibrary::NormalizedDeltaRotator(PlayerCharacter->LockedRot, TargetRot);

	if (-45.0f <= DelRot.Yaw && DelRot.Yaw <= 45.0f)
	{
		return InstructionHold_Forward;
	}
	else if (45.0f < DelRot.Yaw && DelRot.Yaw < 135.0f)
	{
		return InstructionHold_Left;
	}
	else if (-45.0f > DelRot.Yaw && DelRot.Yaw > -135.0f)
	{
		return InstructionHold_Right;
	}
	else
	{
		return InstructionHold_Back;
	}
}

void AFSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AFSPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFSPlayerController::MoveRight);

	InputComponent->BindAxis("LookYaw", this, &AFSPlayerController::LookYaw);
	InputComponent->BindAxis("LookPitch", this, &AFSPlayerController::LookPitch);

	InputComponent->BindAction("W_Key", IE_Pressed, this, &AFSPlayerController::W_Press);
	InputComponent->BindAction("S_Key", IE_Pressed, this, &AFSPlayerController::S_Press);
	InputComponent->BindAction("A_Key", IE_Pressed, this, &AFSPlayerController::A_Press);
	InputComponent->BindAction("D_Key", IE_Pressed, this, &AFSPlayerController::D_Press);
	InputComponent->BindAction("F_Key", IE_Pressed, this, &AFSPlayerController::S_Press);
	InputComponent->BindAction("R_Key", IE_Pressed, this, &AFSPlayerController::R_Press);
	InputComponent->BindAction("Q_Key", IE_Pressed, this, &AFSPlayerController::Q_Press);
	InputComponent->BindAction("E_Key", IE_Pressed, this, &AFSPlayerController::E_Press);

	InputComponent->BindAction("Space_Key", IE_Pressed, this, &AFSPlayerController::Space_Press);
	InputComponent->BindAction("Shift_Key", IE_Pressed, this, &AFSPlayerController::Shift_Press);
	InputComponent->BindAction("Shift_Key", IE_Released, this, &AFSPlayerController::Shift_Release);

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AFSPlayerController::Left_Press);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AFSPlayerController::Left_Release);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AFSPlayerController::Right_Press);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AFSPlayerController::Right_Release);
	InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &AFSPlayerController::Middle_Press);
	InputComponent->BindAction("MiddleMouseButton", IE_Released, this, &AFSPlayerController::Middle_Release);

	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &AFSPlayerController::ScrollUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &AFSPlayerController::ScrollDown);

	InputComponent->BindAction("1_Key", IE_Pressed, this, &AFSPlayerController::Key1_Press);
	InputComponent->BindAction("2_Key", IE_Pressed, this, &AFSPlayerController::Key2_Press);
	InputComponent->BindAction("3_Key", IE_Pressed, this, &AFSPlayerController::Key3_Press);
	InputComponent->BindAction("4_Key", IE_Pressed, this, &AFSPlayerController::Key4_Press);
}

void AFSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	PlayerCharacter = Cast<AFSFischl>(InPawn);
}

void AFSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (LeftMouseHoldingTime >= 0.f)
	{
		LeftMouseHoldingTime += DeltaTime;

		if (LeftMouseHoldingTime >= 0.55)
		{
			LeftMouseHoldingTime = -1.f;
			PlayerCharacter->OnLMouseHolding();
		}
	}

	if (RightMouseHoldingTime >= 0.f)
	{
		RightMouseHoldingTime += DeltaTime;

		if (RightMouseHoldingTime >= 0.7)
		{
			RightMouseHoldingTime = -1.f;
			PlayerCharacter->OnRMouseHolding07();
		}
	}
}

void AFSPlayerController::MoveForward(float Value)
{
	PlayerCharacter->MoveForwardVal = Value;

	if (nullptr == PlayerCharacter->GetActionComp()->GetCurrentAction())
	{
		if (Value != 0.f)
			PlayerCharacter->StopAnimMontage();

		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		PlayerCharacter->AddMovementInput(ControlRot.Vector(), Value);
	}
	else if (PlayerCharacter->GetActionComp()->GetCurrentAction()->GetActionTags().HasTag(FGameplayTag::RequestGameplayTag("Action.Interrupted"))
		&& PlayerCharacter->GetActionComp()->GetCurrentAction()->GetActionName() != "Dodge")
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		PlayerCharacter->AddMovementInput(ControlRot.Vector(), Value);
	}
}

void AFSPlayerController::MoveRight(float Value)
{
	PlayerCharacter->MoveRightVal = Value;

	if (nullptr == PlayerCharacter->GetActionComp()->GetCurrentAction())
	{
		if (Value != 0.f)
			PlayerCharacter->StopAnimMontage();

		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

		PlayerCharacter->AddMovementInput(RightVector, Value);
	}
	else if (PlayerCharacter->GetActionComp()->GetCurrentAction()->GetActionTags().HasTag(FGameplayTag::RequestGameplayTag("Action.Interrupted"))
		&& PlayerCharacter->GetActionComp()->GetCurrentAction()->GetActionName() != "Dodge")
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

		PlayerCharacter->AddMovementInput(RightVector, Value);
	}
}

void AFSPlayerController::LookYaw(float Value)
{
	AddYawInput(Value);
}

void AFSPlayerController::LookPitch(float Value)
{
	if ((GetControlRotation().Pitch < 270.f && GetControlRotation().Pitch >= 180.f && Value > 0) || (GetControlRotation().Pitch > 80.f && GetControlRotation().Pitch <= 90.f && Value < 0))
		return;

	AddPitchInput(Value);
}

void AFSPlayerController::W_Press()
{
	if (PlayerCharacter->bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		InsSeq.Push(GetInsKey(Rot), GetWorld()->TimeSeconds);
	}
}

void AFSPlayerController::S_Press()
{
	if (PlayerCharacter->bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw += 180;
		InsSeq.Push(GetInsKey(Rot), GetWorld()->TimeSeconds);
	}
}

void AFSPlayerController::A_Press()
{
	if (PlayerCharacter->bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw -= 90;
		InsSeq.Push(GetInsKey(Rot), GetWorld()->TimeSeconds);
	}
}

void AFSPlayerController::D_Press()
{
	if (PlayerCharacter->bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw += 90;
		InsSeq.Push(GetInsKey(Rot), GetWorld()->TimeSeconds);
	}
}

void AFSPlayerController::F_Press()
{

}

void AFSPlayerController::R_Press()
{

}

void AFSPlayerController::Q_Press()
{
	if (PlayerCharacter->bIsLocked && PlayerCharacter->LockedActor)
	{
		PlayerCharacter->SwitchTarget(false);
	}
}

void AFSPlayerController::E_Press()
{
	if (PlayerCharacter->bIsLocked && PlayerCharacter->LockedActor)
	{
		PlayerCharacter->SwitchTarget(true);
	}
}

void AFSPlayerController::Shift_Press()
{
	PlayerCharacter->bIsSprinting = true;
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;

	FInstruction Instruction;
	Instruction.Tail = InstructionTail_Shift_Press;
	PlayerCharacter->TryImplementInstruction(Instruction);
}

void AFSPlayerController::Shift_Release()
{
	PlayerCharacter->bIsSprinting = false;

	if (PlayerCharacter->bIsLocked)
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = LOCKED_SPEED;
	else
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;

	//冲刺结束时如果是锁定方向状态，则更改锁定方向为当前方向
	if (!PlayerCharacter->LockedActor)
	{
		PlayerCharacter->LockedRot = PlayerCharacter->GetActorRotation();
	}

	PlayerCharacter->UpdateOrientToMovement(nullptr == PlayerCharacter->GetActionComp()->GetCurrentAction());
}

void AFSPlayerController::Space_Press()
{
	FInstruction Instruction;
	Instruction.Seq = InsSeq.GetSeq(GetWorld()->TimeSeconds);
	Instruction.Hold = GetInsHold();
	Instruction.Tail = InstructionTail_Space_Press;
	PlayerCharacter->TryImplementInstruction(Instruction);
}

void AFSPlayerController::Left_Press()
{
	FInstruction Instruction;
	Instruction.Seq = InsSeq.GetSeq(GetWorld()->TimeSeconds);
	Instruction.Hold = GetInsHold();
	Instruction.Tail = InstructionTail_LeftMouse_Press;
	PlayerCharacter->TryImplementInstruction(Instruction);

	LeftMouseHoldingTime = 0.f;
}

void AFSPlayerController::Left_Release()
{
	if (-1.f == LeftMouseHoldingTime)
	{
		FInstruction Instruction;
		Instruction.Seq = InsSeq.GetSeq(GetWorld()->TimeSeconds);
		Instruction.Hold = GetInsHold();
		Instruction.Tail = InstructionTail_LeftMouse_Release;
		PlayerCharacter->TryImplementInstruction(Instruction);
	}
	LeftMouseHoldingTime = -1.f;
}

void AFSPlayerController::Right_Press()
{
	FInstruction Instruction;
	Instruction.Seq = InsSeq.GetSeq(GetWorld()->TimeSeconds);
	Instruction.Hold = GetInsHold();
	Instruction.Tail = InstructionTail_RightMouse_Press;
	PlayerCharacter->TryImplementInstruction(Instruction);

	RightMouseHoldingTime = 0.f;
}

void AFSPlayerController::Right_Release()
{
	if (-1.f == RightMouseHoldingTime)
	{
		FInstruction Instruction;
		Instruction.Seq = InsSeq.GetSeq(GetWorld()->TimeSeconds);
		Instruction.Hold = GetInsHold();
		Instruction.Tail = InstructionTail_RightMouse_Holding07;
		PlayerCharacter->TryImplementInstruction(Instruction);
	}
	RightMouseHoldingTime = -1.f;
}

void AFSPlayerController::Middle_Press()
{
	PlayerCharacter->ToggleLock();

	if (!PlayerCharacter->bIsSprinting)
	{
		if (PlayerCharacter->bIsLocked)
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = LOCKED_SPEED;
		}
		else
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
		}
	}
}

void AFSPlayerController::Middle_Release()
{

}

void AFSPlayerController::Key1_Press()
{
	PlayerCharacter->ChangeStyle(MilitantStyle_BowFight);
}

void AFSPlayerController::Key2_Press()
{
	PlayerCharacter->ChangeStyle(MilitantStyle_Thunder);
}

void AFSPlayerController::Key3_Press()
{
	PlayerCharacter->ChangeStyle(MilitantStyle_Collaborate);
}

void AFSPlayerController::Key4_Press()
{
	PlayerCharacter->ChangeStyle(MilitantStyle_Wings);
}

void AFSPlayerController::ScrollUp()
{
	if (PlayerCharacter->CameraComp->ScrollScale > 0.4f)
		PlayerCharacter->CameraComp->ScrollScale -= 0.15f;
}

void AFSPlayerController::ScrollDown()
{
	if (PlayerCharacter->CameraComp->ScrollScale < 1.6f)
		PlayerCharacter->CameraComp->ScrollScale += 0.15f;
}
