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

#include "Component/FSCameraComponent.h"
#include "Actor/FSEnemy.h"
#include "Component/FSAttributeComponent.h"
#include "Component/FSActionComponent.h"
#include "Component/FSWeaponComponent.h"
#include "Typedef.h"
#include "Action/FSAction.h"
#include "Projectile/FSProjectileBase.h"
#include "FSPlayerController.h"

// Sets default values
AFSFischl::AFSFischl()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bEnDoubleJump = true;

	bUseControllerRotationYaw = false;

	RootOffset = { 0.f, 0.f, -90.f };

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
	Camera = CreateDefaultSubobject<UFSCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);

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
	EInstruction_Seq Seq = Instruction.Seq;
	EInstruction_Hold Hold = Instruction.Hold;
	EInstruction_Tail Tail = Instruction.Tail;
	float Arg = Instruction.Arg;

	// Jump
	if (EI_Space_Press == Tail)
	{
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			TArray<AActor*> Arr;
			StepComp->GetOverlappingActors(Arr, AActor::StaticClass());

			//One of them is root component
			if (Arr.Num() >= 2)
			{
				if (ActionComp->StartActionByName("StepJump", this, MoveForwardVal || MoveRightVal, Arg)) return true;
			}
			else
			{
				if (bEnDoubleJump)
				{
					bEnDoubleJump = false;
					if (ActionComp->StartActionByName("DoubleJump", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
			}
		}
		else
		{
			if (ActionComp->StartActionByName("Jump", this, MoveForwardVal || MoveRightVal, Arg)) return true;
		}
	}

	if (EI_Shift_Press == Tail)
	{

		if (MilitantStyle == EM_Wings)
		{

		}
		else
		{
			TArray<AActor*> Arr;
			StepComp->GetOverlappingActors(Arr, AActor::StaticClass());

			//One of them is root component
			if (Arr.Num() >= 2)
			{
				ActionComp->StartActionByName("StepDodge", this, true, Arg);
			}
			else if (GetCharacterMovement()->IsMovingOnGround())
			{
				ActionComp->StartActionByName("Dodge", this, true, Arg);
			}
		}
	}

	switch (MilitantStyle)
	{
	case EM_BowFight:
		if (GetMovementComponent()->IsMovingOnGround())
		{
			if (EI_LMouse_Holding07 == Tail)
			{
			}
			else if (EI_RMouse_Holding07 == Tail)
			{
				if (ActionComp->StartActionByName("RoundhouseKick", this, MoveForwardVal || MoveRightVal, Arg)) return true;
			}
			else if (EI_LMouse_Press == Tail)
			{
				if (EI_Forward_Hold == Hold)
				{
					if (ActionComp->StartActionByName("WindChasingArrow", this)) return true;
				}
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("ArrowFan", this)) return true;
				}
				if (ActionComp->bLeftBranchFlag)
				{
					if (ActionComp->StartActionByName("BowAttack_5", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
				else
				{
					if (ActionComp->StartActionByName("NormalAttack", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
			}
			else if (EI_RMouse_Press == Tail)
			{
				if (EI_Forward_Hold == Hold)
				{
					if (ActionComp->StartActionByName("SlidingShovel", this)) return true;
				}
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("EagleKick_Up", this)) return true;
				}
				if (ActionComp->bRightBranchFlag)
				{
					if (ActionComp->StartActionByName("BowAttack_B", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
				else
				{
					if (ActionComp->StartActionByName("BowAttack", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
			}
		}
		else
		{
			if (EI_LMouse_Press == Tail)
			{
				if (ActionComp->bLeftBranchFlag)
				{
					if (ActionComp->StartActionByName("BowAttack_B_2", this, MoveForwardVal || MoveRightVal, Arg)) return true;
				}
				if (ActionComp->StartActionByName("S1_AirNormalAttack", this, MoveForwardVal || MoveRightVal, Arg)) return true;
			}
			else if (EI_RMouse_Press == Tail)
			{
				if (EI_Forward_Hold == Hold)
				{
					if (ActionComp->StartActionByName("EagleKick_Down", this)) return true;
				}
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("EagleKick_Up", this)) return true;
				}
				if (ActionComp->StartActionByName("AirBowAttack", this)) return true;
			}
		}
		break;
	case EM_Thunder:
		if (GetMovementComponent()->IsMovingOnGround())
		{
			if (EI_RMouse_Press == Tail)
			{
				if (EI_BackForward == Seq)
				{
					if (ActionComp->StartActionByName("ThunderingJudgement", this)) return true;
				}
				if (EI_Forward_Hold == Hold)
				{
					if (ActionComp->StartActionByName("ThunderingRetributionNear", this)) return true;
				}
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("ThunderingRetributionFar", this)) return true;
				}
			}
		}
		else
		{
			if (EI_LMouse_Press == Tail)
			{
				if (ActionComp->StartActionByName("AirNormalAttack", this, MoveForwardVal || MoveRightVal, Arg)) return true;
			}
		}
		break;
	case EM_Collaborate:
		break;
	case EM_Wings:
		break;
	default:
		break;
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

void AFSFischl::OnLMouseHolding07()
{

}

void AFSFischl::OnRMouseHolding07()
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(FXBlackCharge, GetMesh(), "LeftLegEndSocket", FVector(0.f, 20.f, 0.f), FRotator(), EAttachLocation::SnapToTargetIncludingScale, true);
}

// Called when the game starts or when spawned
void AFSFischl::BeginPlay()
{
	Super::BeginPlay();
}

void AFSFischl::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFSFischl::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bEnDoubleJump = true;
}

bool AFSFischl::SetOrientToMovement(bool bVal)
{
	return GetCharacterMovement()->bOrientRotationToMovement = bVal && !bIsLocked || bVal && bIsSprinting;
}

void AFSFischl::ToggleLock()
{
	bIsLocked = !bIsLocked;

	SetOrientToMovement(nullptr == ActionComp->CurrentAction);

	if (bIsLocked) {
		AActor* NearestActor = nullptr;
		float NearestArg;

		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFSEnemy::StaticClass(), Arr);
		for (TActorIterator<AActor> It(GetWorld(), AFSEnemy::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;

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
	AActor* NearestActor = LockedActor;
	float NearestYaw;

	FRotator FirstRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
	for (TActorIterator<AActor> It(GetWorld(), AFSEnemy::StaticClass()); It; ++It)
	{
		AActor* Actor = *It;

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
	case EM_BowFight:
		return CompoundBowComp;
	case EM_Thunder:
		return ThunderingPulseComp;
	case EM_Collaborate:
		return MitternachtsWaltzComp;
	case EM_Wings:
		return AlleyHunterComp;
	default:
		return nullptr;
	}
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
			//Set Actor Rotation
			CONSTEXPR float MaxRotYaw = 3.0f;

			LockedRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
			FRotator CurRot = GetActorRotation();
			FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(LockedRot, CurRot);

			if (!GetCharacterMovement()->bOrientRotationToMovement && nullptr == ActionComp->CurrentAction)
			{
				if (DeltaRot.Yaw > MaxRotYaw)
				{
					CurRot.Yaw += MaxRotYaw;
					SetActorRotation(CurRot);
				}
				else if (DeltaRot.Yaw < -MaxRotYaw)
				{
					CurRot.Yaw -= MaxRotYaw;
					SetActorRotation(CurRot);
				}
				else
				{
					LockedRot.Pitch = 0.f;
					LockedRot.Roll = 0.f;
					SetActorRotation(LockedRot);
				}
			}

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