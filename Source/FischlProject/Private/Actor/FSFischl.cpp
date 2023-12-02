// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSFischl.h"
#include "Component/FSCameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Actor/FSEnemy.h"
#include "Component/FSAttributeComponent.h"
#include "Component/FSActionComponent.h"
#include "Component/FSWeaponComponent.h"
#include "Components/WindDirectionalSourceComponent.h"
#include "Typedef.h"
#include <GameplayTagContainer.h>
#include "Action/FSAction.h"
#include "Projectile/FSProjectileBase.h"
#include "NiagaraFunctionLibrary.h"

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

// Called when the game starts or when spawned
void AFSFischl::BeginPlay()
{
	Super::BeginPlay();
}

void AFSFischl::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFSFischl::MoveForward(float Value)
{
	MoveForwardVal = Value;

	if (nullptr == ActionComp->CurrentAction)
	{
		if (Value != 0.f)
			StopAnimMontage();

		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		AddMovementInput(ControlRot.Vector(), Value);
	}
	else if (ActionComp->CurrentAction->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Interrupted")))
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		AddMovementInput(ControlRot.Vector(), Value);
	}
}

void AFSFischl::MoveRight(float Value)
{
	MoveRightVal = Value;

	if (nullptr == ActionComp->CurrentAction)
	{
		if(Value != 0.f)
			StopAnimMontage();

		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

		AddMovementInput(RightVector, Value);
	}
	else if (ActionComp->CurrentAction->ActionTags.HasTag(FGameplayTag::RequestGameplayTag("Action.Interrupted")))
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;
		ControlRot.Roll = 0.0f;

		FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

		AddMovementInput(RightVector, Value);
	}
}

void AFSFischl::LookYaw(float Value)
{
	APawn::AddControllerYawInput(Value);
}

void AFSFischl::LookPitch(float Value)
{
	if ((GetControlRotation().Pitch < 270.f && GetControlRotation().Pitch >= 180.f && Value > 0) || (GetControlRotation().Pitch > 80.f && GetControlRotation().Pitch <= 90.f && Value < 0))
		return;

	APawn::AddControllerPitchInput(Value);
}

void AFSFischl::W_Press()
{
	if (bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		InsSeq.Instruction[InsSeq.Pos] = GetInsHead(Rot);
		InsSeq.InstigateTime[InsSeq.Pos] = GetWorld()->TimeSeconds;

		if (++InsSeq.Pos >= MAX_INS_NUM)
		{
			InsSeq.Pos = 0;
		}
	}
}

void AFSFischl::S_Press()
{
	if (bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw += 180;
		InsSeq.Instruction[InsSeq.Pos] = GetInsHead(Rot);
		InsSeq.InstigateTime[InsSeq.Pos] = GetWorld()->TimeSeconds;

		if (++InsSeq.Pos >= MAX_INS_NUM)
		{
			InsSeq.Pos = 0;
		}
	}
}

void AFSFischl::A_Press()
{
	if (bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw -= 90;
		InsSeq.Instruction[InsSeq.Pos] = GetInsHead(Rot);
		InsSeq.InstigateTime[InsSeq.Pos] = GetWorld()->TimeSeconds;

		if (++InsSeq.Pos >= MAX_INS_NUM)
		{
			InsSeq.Pos = 0;
		}
	}
}

void AFSFischl::D_Press()
{
	if (bIsLocked)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw += 90;
		InsSeq.Instruction[InsSeq.Pos] = GetInsHead(Rot);
		InsSeq.InstigateTime[InsSeq.Pos] = GetWorld()->TimeSeconds;

		if (++InsSeq.Pos >= MAX_INS_NUM)
		{
			InsSeq.Pos = 0;
		}
	}
}

EInstruction_Head AFSFischl::GetInsHead(FRotator& Rot)
{
	if (LockedActor)
	{
		LockedRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
	}
	FRotator DelRot = UKismetMathLibrary::NormalizedDeltaRotator(LockedRot, Rot);

	if (-45.0f <= DelRot.Yaw && DelRot.Yaw <= 45.0f)
	{
		return EI_Forward;
	}
	else if (45.0f < DelRot.Yaw && DelRot.Yaw < 135.0f)
	{
		return EI_Left;
	}
	else if (-45.0f > DelRot.Yaw && DelRot.Yaw > -135.0f)
	{
		return EI_Right;
	}
	else
	{
		return EI_Back;
	}
}

EInstruction_Hold AFSFischl::GetInsHold()
{
	if (!bIsLocked || (abs(MoveForwardVal) <= KINDA_SMALL_NUMBER && abs(MoveRightVal) <= KINDA_SMALL_NUMBER))
	{
		return EI_None;
	}

	if (LockedActor)
	{
		LockedRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
	}

	FRotator TargetRot = GetControlRotation();
	TargetRot.Yaw += FVector(MoveForwardVal, MoveRightVal, 0.0f).Rotation().Yaw;

	FRotator DelRot = UKismetMathLibrary::NormalizedDeltaRotator(LockedRot, TargetRot);

	if (-45.0f <= DelRot.Yaw && DelRot.Yaw <= 45.0f)
	{
		return EI_Forward_Hold;
	}
	else if (45.0f < DelRot.Yaw && DelRot.Yaw < 135.0f)
	{
		return EI_Left_Hold;
	}
	else if (-45.0f > DelRot.Yaw && DelRot.Yaw > -135.0f)
	{
		return EI_Right_Hold;
	}
	else
	{
		return EI_Back_Hold;
	}
}

float AFSFischl::GetInsDir()
{
	float CtrlDir = GetControlRotation().Yaw;

	if (MoveForwardVal || MoveRightVal)
		return CtrlDir + FVector(MoveForwardVal, MoveRightVal, 0.0f).Rotation().Yaw;
	else
		return GetActorRotation().Yaw;
}

bool AFSFischl::StartActionByInstruction(InstructionSeq& Seq, EInstruction_Hold Hold, EInstruction_Tail Tail, float Arg)
{
	// Jump
	if (EI_Space_Press == Tail)
	{
		if (GetCharacterMovement()->IsFalling())
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

	// Attack
	switch (MilitantStyle)
	{
	case EM_BowFight:
		if (GetMovementComponent()->IsFalling())
		{
			if (EI_LMouse_Press == Tail)
			{

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
			}
		}
		else
		{
			if (EI_LMouse_Press == Tail)
			{
				if (EI_Forward_Hold == Hold)
				{
					if (ActionComp->StartActionByName("WindChasingArrow", this)) return true;
				}
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("ArrowFan", this)) return true;
				}
				if (ActionComp->StartActionByName("NormalAttack", this, false, Arg)) return true;
			}
			else if(EI_RMouse_Press == Tail)
			{
				if (EI_Back_Hold == Hold)
				{
					if (ActionComp->StartActionByName("EagleKick_Up", this)) return true;
				}
				if (ActionComp->StartActionByName("BowAttack", this, false, Arg)) return true;
			}
		}
		break;
	case EM_Thunder:
		if (GetMovementComponent()->IsFalling())
		{
			if (EI_LMouse_Press == Tail)
			{

			}
		}
		else
		{
			if (EI_RMouse_Press == Tail)
			{
				if (EI_Back == Seq.LookBackValidIns(GetWorld()->TimeSeconds, 2) &&
					EI_Forward == Seq.LookBackValidIns(GetWorld()->TimeSeconds))
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

void AFSFischl::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bEnDoubleJump = true;
}

bool AFSFischl::SetOrientToMovement(bool bVal)
{
	return GetCharacterMovement()->bOrientRotationToMovement = bVal && !bIsLocked || bVal && bIsSprinting;
}

void AFSFischl::ShootArrow()
{
	if (ensure(ProjectileClass))
	{
		FVector Location = GetMesh()->GetSocketLocation("WeaponSocket");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FRotator Rot;
		if (bIsLocked && LockedActor)
		{
			Rot = UKismetMathLibrary::FindLookAtRotation(Location, LockedActor->GetActorLocation());

		}
		else
		{
			Rot = GetActorRotation();
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShootVFX, Location, Rot, FVector(1.f), true, true, ENCPoolMethod::None, true);

		FTransform SpawnTM = FTransform(Rot, Location);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}

void AFSFischl::F_Press()
{

}

void AFSFischl::R_Press()
{

}

void AFSFischl::Q_Press()
{
	if (bIsLocked && LockedActor)
	{
		SwitchTarget(false);
	}
}

void AFSFischl::E_Press()
{
	if (bIsLocked && LockedActor)
	{
		SwitchTarget(true);
	}
}

void AFSFischl::Shift_Press()
{
	bIsSprinting = true;

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
			ActionComp->StartActionByName("StepDodge", this, true, GetInsDir());
		}
		else if(!GetCharacterMovement()->IsFalling())
		{
			ActionComp->StartActionByName("Dodge", this, true, GetInsDir());
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
}

void AFSFischl::Shift_Release()
{
	bIsSprinting = false;

	if(bIsLocked)
		GetCharacterMovement()->MaxWalkSpeed = LOCKED_SPEED;
	else
		GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;

	//冲刺结束时如果是锁定方向状态，则更改锁定方向为当前方向
	LockedRot = GetActorRotation();

	SetOrientToMovement(nullptr == ActionComp->CurrentAction);
}

void AFSFischl::Space_Press()
{
	StartActionByInstruction(InsSeq, GetInsHold(), EI_Space_Press, GetInsDir());
}

void AFSFischl::Left_Press()
{
	StartActionByInstruction(InsSeq, GetInsHold(), EI_LMouse_Press, GetInsDir());
}

void AFSFischl::Left_Release()
{
	StartActionByInstruction(InsSeq, GetInsHold(), EI_LMouse_Release);
}

void AFSFischl::Right_Press()
{
	StartActionByInstruction(InsSeq, GetInsHold(), EI_RMouse_Press, GetInsDir());
}

void AFSFischl::Right_Release()
{
	StartActionByInstruction(InsSeq, GetInsHold(), EI_RMouse_Release);
}

void AFSFischl::Middle_Press()
{
	ToggleLock();

	if (!bIsSprinting)
	{
		if (bIsLocked)
		{
			GetCharacterMovement()->MaxWalkSpeed = LOCKED_SPEED;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
		}
	}
}

void AFSFischl::Middle_Release()
{

}

void AFSFischl::Key1_Press()
{
	if(MilitantStyle) GetWeaponByStyle(MilitantStyle)->SetVisibility(false);
	MilitantStyle = EM_BowFight;
	GetWeaponByStyle(MilitantStyle)->SetVisibility(true);
}

void AFSFischl::Key2_Press()
{
	if (MilitantStyle) GetWeaponByStyle(MilitantStyle)->SetVisibility(false);
	MilitantStyle = EM_Thunder;
	GetWeaponByStyle(MilitantStyle)->SetVisibility(true);
}

void AFSFischl::Key3_Press()
{
	if (MilitantStyle) GetWeaponByStyle(MilitantStyle)->SetVisibility(false);
	MilitantStyle = EM_Collaborate;
	GetWeaponByStyle(MilitantStyle)->SetVisibility(true);
}

void AFSFischl::Key4_Press()
{
	if (MilitantStyle) GetWeaponByStyle(MilitantStyle)->SetVisibility(false);
	MilitantStyle = EM_Wings;
	GetWeaponByStyle(MilitantStyle)->SetVisibility(true);
}

void AFSFischl::ScrollUp()
{
	if (Camera->ScrollScale > 0.4f)
		Camera->ScrollScale -= 0.15f;
}

void AFSFischl::ScrollDown()
{
	if (Camera->ScrollScale < 1.6f)
		Camera->ScrollScale += 0.15f;
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
			LockedRot = FRotator(0.f, GetInsDir(), 0.f);
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

// Called to bind functionality to input
void AFSFischl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFSFischl::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFSFischl::MoveRight);

	PlayerInputComponent->BindAxis("LookYaw", this, &AFSFischl::LookYaw);
	PlayerInputComponent->BindAxis("LookPitch", this, &AFSFischl::LookPitch);

	PlayerInputComponent->BindAction("W_Key", IE_Pressed, this, &AFSFischl::W_Press);
	PlayerInputComponent->BindAction("S_Key", IE_Pressed, this, &AFSFischl::S_Press);
	PlayerInputComponent->BindAction("A_Key", IE_Pressed, this, &AFSFischl::A_Press);
	PlayerInputComponent->BindAction("D_Key", IE_Pressed, this, &AFSFischl::D_Press);
	PlayerInputComponent->BindAction("F_Key", IE_Pressed, this, &AFSFischl::S_Press);
	PlayerInputComponent->BindAction("R_Key", IE_Pressed, this, &AFSFischl::R_Press);
	PlayerInputComponent->BindAction("Q_Key", IE_Pressed, this, &AFSFischl::Q_Press);
	PlayerInputComponent->BindAction("E_Key", IE_Pressed, this, &AFSFischl::E_Press);

	PlayerInputComponent->BindAction("Space_Key", IE_Pressed, this, &AFSFischl::Space_Press);
	PlayerInputComponent->BindAction("Shift_Key", IE_Pressed, this, &AFSFischl::Shift_Press);
	PlayerInputComponent->BindAction("Shift_Key", IE_Released, this, &AFSFischl::Shift_Release);

	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AFSFischl::Left_Press);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Released, this, &AFSFischl::Left_Release);
	PlayerInputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AFSFischl::Right_Press);
	PlayerInputComponent->BindAction("RightMouseButton", IE_Released, this, &AFSFischl::Right_Release);
	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &AFSFischl::Middle_Press);
	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Released, this, &AFSFischl::Middle_Release);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AFSFischl::ScrollUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AFSFischl::ScrollDown);

	PlayerInputComponent->BindAction("1_Key", IE_Pressed, this, &AFSFischl::Key1_Press);
	PlayerInputComponent->BindAction("2_Key", IE_Pressed, this, &AFSFischl::Key2_Press);
	PlayerInputComponent->BindAction("3_Key", IE_Pressed, this, &AFSFischl::Key3_Press);
	PlayerInputComponent->BindAction("4_Key", IE_Pressed, this, &AFSFischl::Key4_Press);
}

