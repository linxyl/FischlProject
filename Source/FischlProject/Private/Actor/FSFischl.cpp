// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSFischl.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/WindDirectionalSourceComponent.h"
#include "GameplayTagContainer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Component/FSCameraComponent.h"
#include "Actor/FSEnemy.h"
#include "Component/FSAttributeComponent.h"
#include "Component/FSActionComponent.h"
#include "Component/FSWeaponComponent.h"
#include "Typedef.h"
#include "Action/FSAction.h"
#include "Projectile/FSProjectileBase.h"
#include "FSPlayerController.h"
#include "Component/FSBuffComponent.h"
#include "Component/FSShootComponent.h"
#include "Actor/FSOz.h"

// Sets default values
AFSFischl::AFSFischl()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bEnDoubleJump = true;

	bUseControllerRotationYaw = false;

	// Init weapon component
	FRotator WeaponRot(0.f, -90.f, -90.f);
	CompoundBowComp = CreateDefaultSubobject<UFSWeaponComponent>("CompoundBowComp");
	CompoundBowComp->SetupAttachment(GetMesh(), "WeaponSocket");
	CompoundBowComp->SetRelativeLocationAndRotation(FVector(8.f, -6.f, 0.f), WeaponRot.Quaternion());
	CompoundBowComp->SetVisibility(false);
	ThunderingPulseComp = CreateDefaultSubobject<UFSWeaponComponent>("ThunderingPulse");
	ThunderingPulseComp->SetupAttachment(GetMesh(), "WeaponSocket");
	ThunderingPulseComp->SetRelativeLocationAndRotation(FVector(8.f, -6.f, 0.f), WeaponRot.Quaternion());
	ThunderingPulseComp->SetVisibility(false);
	MitternachtsWaltzComp = CreateDefaultSubobject<UFSWeaponComponent>("MitternachtsWaltzComp");
	MitternachtsWaltzComp->SetupAttachment(GetMesh(), "WeaponSocket");
	MitternachtsWaltzComp->SetRelativeLocationAndRotation(FVector(8.f, -6.f, 0.f), WeaponRot.Quaternion());
	MitternachtsWaltzComp->SetVisibility(false);
	AlleyHunterComp = CreateDefaultSubobject<UFSWeaponComponent>("AlleyHunterComp");
	AlleyHunterComp->SetupAttachment(GetMesh(), "WeaponSocket");
	AlleyHunterComp->SetRelativeLocationAndRotation(FVector(8.f, -6.f, 0.f), WeaponRot.Quaternion());
	AlleyHunterComp->SetVisibility(false);

	// Init camera component
	CameraComp = CreateDefaultSubobject<UFSCameraComponent>("Camera");
	CameraComp->SetupAttachment(RootComponent);

	// Init jump check component
	StepComp = CreateDefaultSubobject<UCapsuleComponent>("StepComp");
	StepComp->SetupAttachment(RootComponent);
	StepComp->SetRelativeLocation(FVector(0, 0, -50.0f));
	StepComp->SetRelativeScale3D(FVector(3.0f, 3.0f, 2.25f));
	StepComp->SetCollisionProfileName("FootSetp");

	// Init movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->JumpZVelocity = 1800.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.0f, 0.f);
	GetCharacterMovement()->MaxAcceleration = 20000.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 0.7f;
}

void AFSFischl::TryImplementInstruction(FInstruction Instruction)
{
	if (!StartActionByInstruction(Instruction))
	{
		NextInstruction = Instruction;
		NextInstructionTime = GetWorld()->TimeSeconds;
	}
}

bool AFSFischl::StartActionByInstruction(FInstruction Instruction)
{
	EInstructionSeq Seq = Instruction.Seq;
	EInstructionHold Hold = Instruction.Hold;
	EInstructionTail Tail = Instruction.Tail;

	// Jump
	if (InstructionTail_Space_Press == Tail)
	{
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			TArray<AActor*> Arr;
			StepComp->GetOverlappingActors(Arr, AActor::StaticClass());

			//One of them is root component
			if (Arr.Num() >= 2)
			{
				if (GetActionComp()->StartActionByName("StepJump", this)) return true;
			}
			else
			{
				if (bEnDoubleJump)
				{
					bEnDoubleJump = false;
					if (GetActionComp()->StartActionByName("DoubleJump", this)) return true;
				}
			}
		}
		else
		{
			if (GetActionComp()->StartActionByName("Jump", this)) return true;
		}
	}

	if (InstructionTail_Shift_Press == Tail)
	{
		if (MilitantStyle == MilitantStyle_Wings)
		{

		}
		else
		{
			TArray<AActor*> Arr;
			StepComp->GetOverlappingActors(Arr, AActor::StaticClass());

			//One of them is root component
			if (Arr.Num() >= 2)
			{
				if(GetActionComp()->StartActionByName("StepDodge", this)) return true;
			}
			else if (GetCharacterMovement()->IsMovingOnGround())
			{
				if(GetActionComp()->StartActionByName("Dodge", this)) return true;
			}
		}
	}

	if (InstructionTail_LeftMouse_Release == Tail)
	{
		if (GetBuffComp()->FindBuff("TripleShoot"))
		{
			if (GetActionComp()->StartActionByName("TripleShoot", this)) return true;
		}
		if (GetBuffComp()->FindAndUseBuff("BoltsOfDownfall"))
		{
			if (GetActionComp()->StartActionByName("BoltsOfDownfall", this)) return true;
		}
		if (GetBuffComp()->FindAndUseBuff("ShadowlessArrow"))
		{
			if (GetActionComp()->StartActionByName("ShadowlessArrow", this)) return true;
		}
	}

	switch (MilitantStyle)
	{
	case MilitantStyle_BowFight:
		return StartBowFightAction(Instruction);
	case MilitantStyle_Thunder:
		return StartThunderAction(Instruction);
	case MilitantStyle_Collaborate:
		return StartCollaborateAction(Instruction);
	case MilitantStyle_Wings:
		return StartWingsAction(Instruction);
	default:
		break;
	}

	return false;
}

bool AFSFischl::StartBowFightAction(FInstruction Instruction)
{
	EInstructionSeq Seq = Instruction.Seq;
	EInstructionHold Hold = Instruction.Hold;
	EInstructionTail Tail = Instruction.Tail;

	if (GetMovementComponent()->IsMovingOnGround())
	{
		if (InstructionTail_RightMouse_Holding07 == Tail)
		{
			if (GetActionComp()->StartActionByName("RoundhouseKick", this)) return true;
		}
		else if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (InstructionHold_Forward == Hold)
			{
				if (GetActionComp()->StartActionByName("WindChasingArrow", this)) return true;
			}
			if (InstructionHold_Back == Hold)
			{
				if (GetActionComp()->StartActionByName("ArrowFan", this)) return true;
			}
			if (GetActionComp()->bLeftBranchFlag)
			{
				if (GetActionComp()->StartActionByName("BowAttack_5", this)) return true;
			}
			if (GetActionComp()->StartActionByName("NormalAttack", this)) return true;
		}
		else if (InstructionTail_RightMouse_Press == Tail)
		{
			if (InstructionHold_Forward == Hold)
			{
				if (GetActionComp()->StartActionByName("SlidingShovel", this)) return true;
			}
			if (InstructionHold_Back == Hold)
			{
				if (GetActionComp()->StartActionByName("EagleKick_Up", this)) return true;
			}
			if (GetActionComp()->bRightBranchFlag)
			{
				if (GetActionComp()->StartActionByName("BowAttack_B", this)) return true;
			}
			if (GetActionComp()->StartActionByName("BowAttack", this)) return true;
		}
	}
	else
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->bLeftBranchFlag)
			{
				if (GetActionComp()->StartActionByName("BowAttack_B_2", this)) return true;
			}
			if (GetActionComp()->StartActionByName("AirNormalAttack", this)) return true;
		}
		else if (InstructionTail_RightMouse_Press == Tail)
		{
			if (InstructionHold_Forward == Hold)
			{
				if (GetActionComp()->StartActionByName("EagleKick_Down", this)) return true;
			}
			if (InstructionHold_Back == Hold)
			{
				if (GetActionComp()->StartActionByName("EagleKick_Up", this)) return true;
			}
			if (GetActionComp()->StartActionByName("AirBowAttack", this)) return true;
		}
	}
	return false;
}

bool AFSFischl::StartThunderAction(FInstruction Instruction)
{
	EInstructionSeq Seq = Instruction.Seq;
	EInstructionHold Hold = Instruction.Hold;
	EInstructionTail Tail = Instruction.Tail;

	if (GetMovementComponent()->IsMovingOnGround())
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("NormalAttack", this)) return true;
		}
		else if (InstructionTail_RightMouse_Press == Tail)
		{
			if (InstructionSeq_BackForward == Seq)
			{
				if (GetActionComp()->StartActionByName("ThunderingJudgement", this)) return true;
			}
			if (InstructionHold_Forward == Hold)
			{
				if (GetActionComp()->StartActionByName("ThunderingRetributionNear", this)) return true;
			}
			if (InstructionHold_Back == Hold)
			{
				if (GetActionComp()->StartActionByName("ThunderingRetributionFar", this)) return true;
			}
		}
	}
	else
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("AirNormalAttack", this)) return true;
		}
		else if(InstructionTail_RightMouse_Press == Tail)
		{
			if (InstructionHold_Forward == Hold)
			{
				if (GetActionComp()->StartActionByName("LockOfThunderForward", this)) return true;
			}
			if (InstructionHold_Back == Hold)
			{
				if (GetActionComp()->StartActionByName("LockOfThunderBack", this)) return true;
			}
		}
	}
	return false;
}

bool AFSFischl::StartCollaborateAction(FInstruction Instruction)
{
	EInstructionSeq Seq = Instruction.Seq;
	EInstructionHold Hold = Instruction.Hold;
	EInstructionTail Tail = Instruction.Tail;

	if (GetMovementComponent()->IsMovingOnGround())
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("NormalAttack", this)) return true;
		}
	}
	else
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("AirNormalAttack", this)) return true;
		}
	}

	if (InstructionTail_RightMouse_Press == Tail)
	{
		if (InstructionHold_Forward == Hold)
		{
			if (GetActionComp()->StartActionByName("Dive", this)) return true;
		}
		if (InstructionHold_Back == Hold)
		{
			if (GetActionComp()->StartActionByName("Splash", this)) return true;
		}
		if (GetActionComp()->StartActionByName("Nightrider", this)) return true;
	}

	return false;
}

bool AFSFischl::StartWingsAction(FInstruction Instruction)
{
	EInstructionSeq Seq = Instruction.Seq;
	EInstructionHold Hold = Instruction.Hold;
	EInstructionTail Tail = Instruction.Tail;

	if (GetMovementComponent()->IsMovingOnGround())
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("NormalAttack", this)) return true;
		}
	}
	else
	{
		if (InstructionTail_LeftMouse_Press == Tail)
		{
			if (GetActionComp()->StartActionByName("AirNormalAttack", this)) return true;
		}
	}
	return false;
}

void AFSFischl::StartNextInstruction()
{
	if (GetWorld()->TimeSeconds - NextInstructionTime <= 0.4)
	{
		StartActionByInstruction(NextInstruction);
	}
	NextInstructionTime = 0.f;
}

void AFSFischl::OnLMouseHolding()
{
	switch (MilitantStyle)
	{
	case MilitantStyle_None:
		break;
	case MilitantStyle_BowFight:
		GetBuffComp()->AddBuff("TripleShoot");
		break;
	case MilitantStyle_Thunder:
		GetBuffComp()->AddBuff("BoltsOfDownfall");
		break;
	case MilitantStyle_Collaborate:
		break;
	case MilitantStyle_Wings:
		break;
	default:
		break;
	}
}

void AFSFischl::OnRMouseHolding07()
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(FXBlackCharge, GetMesh(), "LeftLegEndSocket", FVector(0.f, 20.f, 0.f), FRotator(), EAttachLocation::SnapToTargetIncludingScale, true);
}

void AFSFischl::ChangeStyle(EMilitantStyle Style)
{
	if (MilitantStyle)
		GetWeaponByStyle(MilitantStyle)->SetVisibility(false);
	MilitantStyle = Style;
	WeaponComponent = GetWeaponByStyle(MilitantStyle);
	WeaponComponent->SetVisibility(true);

	GetBuffComp()->RemoveBuff("BowFight");
	GetBuffComp()->RemoveBuff("Thunder");
	GetBuffComp()->RemoveBuff("Collaborate");
	GetBuffComp()->RemoveBuff("Wings");
	switch (Style)
	{
	case MilitantStyle_BowFight:
		GetBuffComp()->AddBuff("BowFight");
		GetShootComp()->SelectProjectile(0);
		break;
	case MilitantStyle_Thunder:
		GetBuffComp()->AddBuff("Thunder");
		GetShootComp()->SelectProjectile(1);
		break;
	case MilitantStyle_Collaborate:
		GetBuffComp()->AddBuff("Collaborate");
		GetShootComp()->SelectProjectile(0);
		break;
	case MilitantStyle_Wings:
		GetBuffComp()->AddBuff("Wings");
		GetShootComp()->SelectProjectile(2);
		break;
	default:
		break;
	}

	Oz->DisplayFX();
}

void AFSFischl::ToggleLock()
{
	bIsLocked = !bIsLocked;
	if (!bIsLocked)
	{
		LockedActor = nullptr;
	}

	UpdateOrientToMovement(nullptr == GetActionComp()->GetCurrentAction());

	if (bIsLocked) {
		APawn* NearestActor = nullptr;
		float NearestArg;

		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFSEnemy::StaticClass(), Arr);
		for (TActorIterator<APawn> It(GetWorld(), AFSEnemy::StaticClass()); It; ++It)
		{
			APawn* Actor = *It;

			if ((Actor->GetActorLocation() - GetActorLocation()).SizeSquared() > 20000000)
			{
				continue;
			}

			FRotator CtrlRot = GetControlRotation();
			FRotator DirRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Actor->GetActorLocation());
			FRotator DelRot = UKismetMathLibrary::NormalizedDeltaRotator(CtrlRot, DirRot);
			float Temp1 = UKismetMathLibrary::Square(DelRot.Roll) + UKismetMathLibrary::Square(DelRot.Pitch) + UKismetMathLibrary::Square(DelRot.Yaw);

			FVector TempVec = Actor->GetActorLocation() - GetActorLocation();
			float Temp2 = TempVec.SizeSquared();

			if (nullptr == NearestActor || (Temp1 * Temp2) < NearestArg)
			{
				NearestActor = Actor;
				NearestArg = Temp1 * Temp2;
			}
		}
		LockedActor = NearestActor;

		if (!NearestActor)
		{
			LockedRot = FRotator(0.f, GetActorRotation().Yaw, 0.f);
		}
	}
}

void AFSFischl::SwitchTarget(bool bRight)
{
	APawn* NearestActor = LockedActor;
	float NearestYaw;

	FRotator FirstRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
	for (TActorIterator<APawn> It(GetWorld(), AFSEnemy::StaticClass()); It; ++It)
	{
		APawn* Actor = *It;

		if (Actor == LockedActor)
		{
			continue;
		}

		FRotator ActorRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Actor->GetActorLocation());

		float DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(FirstRot, ActorRot).Yaw;

		DeltaYaw += DeltaYaw < 0.0f ? 360 : 0;

		if (LockedActor == NearestActor || (bRight && DeltaYaw > NearestYaw) || (!bRight && DeltaYaw < NearestYaw))
		{
			NearestActor = Actor;
			NearestYaw = DeltaYaw;
		}
	}

	LockedActor = NearestActor;
}

UFSWeaponComponent* AFSFischl::GetWeaponByStyle(EMilitantStyle Style)
{
	switch (Style)
	{
	case MilitantStyle_BowFight:
		return CompoundBowComp;
	case MilitantStyle_Thunder:
		return ThunderingPulseComp;
	case MilitantStyle_Collaborate:
		return MitternachtsWaltzComp;
	case MilitantStyle_Wings:
		return AlleyHunterComp;
	default:
		return nullptr;
	}
}

// Called when the game starts or when spawned
void AFSFischl::BeginPlay()
{
	Super::BeginPlay();

	FTransform SpawnTM = FTransform(GetActorRotation(), GetActorLocation() + FVector(0.f, 0.f, 100.f));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	Oz = GetWorld()->SpawnActor<AFSOz>(OzClass, SpawnTM, SpawnParams);
	Oz->SpawnDefaultController();

	IgnoredActors.Add(Oz);
}

void AFSFischl::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AFSFischl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Lock perspective
	if (bIsLocked)
	{
		if (LockedActor)
		{
			//Unlock when out of range
			if ((LockedActor->GetActorLocation() - GetActorLocation()).SizeSquared() > 30000000)
			{
				ToggleLock();
			}
		}
		else
		{
			//Set Actor Rotation
			if (!bIsSprinting)
			{
				SetActorRotation(LockedRot);
			}
		}
	}
}

void AFSFischl::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bEnDoubleJump = true;
}

bool AFSFischl::UpdateOrientToMovement(bool bVal)
{
	return GetCharacterMovement()->bOrientRotationToMovement = (bVal && !bIsLocked) || (bVal && bIsSprinting && !GetActionComp()->GetCurrentAction());
}