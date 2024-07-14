// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSCharacter.h"

#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/FSAttributeComponent.h"
#include "Component/FSActionComponent.h"
#include "FSFunctionLibrary.h"
#include "Typedef.h"
#include "Component/FSWeaponComponent.h"
#include "Component/FSShootComponent.h"
#include "Component/FSBuffComponent.h"
#include "Component/FSEffectComponent.h"

// Sets default values
AFSCharacter::AFSCharacter()
{
	// Set Mesh
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName("Body");
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation_Direct(FRotator(0.f, -90.f, 0.f));

	AttributeComp = CreateDefaultSubobject<UFSAttributeComponent>("AttributeComp");
	AttributeComp->SetMaxVal(100.0f, 100.0f, 100.0f);
	ActionComp = CreateDefaultSubobject<UFSActionComponent>("ActionComp");
	BuffComp = CreateDefaultSubobject<UFSBuffComponent>("BuffComp");
	EffectComp = CreateDefaultSubobject<UFSEffectComponent>("EffectComp");
	ShootComp = CreateDefaultSubobject<UFSShootComponent>("ShootComp");

	RootOffset = { 0.f, 0.f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() };

	//// Set Effect
	//UObject* loadVFXObj = StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, TEXT("NiagaraSystem'/Game/Effects/FX_HunmanInjured.FX_HunmanInjured'"));
	//if (loadVFXObj)
	//{
	//	InjuredVFX = Cast<UNiagaraSystem>(loadVFXObj);
	//}
}

void AFSCharacter::RotateByLocked(float MaxRotYaw)
{
	//Lock perspective
	if (bIsLocked && LockedActor)
	{
		//Set Actor Rotation
		FRotator CurRot = GetActorRotation();
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(LockedRot, CurRot);

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
			SetActorRotation(LockedRot);
		}
	}
}

void AFSCharacter::SetWeaponVisible(bool b /*= true*/)
{
	if (ensure(WeaponComponent))
	{
		WeaponComponent->SetVisibility(b);
	}
}

bool AFSCharacter::UpdateOrientToMovement(bool bVal)
{
	return GetCharacterMovement()->bOrientRotationToMovement = bVal && !bIsLocked;
}

void AFSCharacter::ReceivedDamage(AActor* InstigatorActor, FVector ImpactPoint, FDamageParam DamageParam)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		GetEffectComp()->InjuredVFX,
		ImpactPoint,
		InstigatorActor->GetActorRotation() + DamageParam.Direction,
		FVector(1.f),
		true,
		true,
		ENCPoolMethod::None,
		true
	);
}

void AFSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnReceivedDamage.AddDynamic(this, &AFSCharacter::ReceivedDamage);
}

// Called every frame
void AFSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
	else if (!GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->GravityScale < FAST_GRAVITY)
	{
		GetCharacterMovement()->GravityScale += DeltaTime * 0.5;
	}

	UpdateLockedRot();
	if (!GetCharacterMovement()->bOrientRotationToMovement && nullptr == ActionComp->GetCurrentAction() && GetCharacterMovement()->IsMovingOnGround())
	{
		RotateByLocked();
	}
}

void AFSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	AFSCharacter* Character = Cast<AFSCharacter>(Hit.Actor);
	if (!Character)
	{
		ActionComp->StopCurrentAction(this);
		StopAnimMontage(GetCurrentMontage());

		GetCharacterMovement()->GravityScale = DEFAULT_GRAVITY;

		//bIsLaunching = false;
	}
}

void AFSCharacter::UpdateLockedRot()
{
	if (bIsLocked && LockedActor)
	{
		LockedRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedActor->GetActorLocation());
	}
	else
	{
		LockedRot = GetActorRotation();
	}

	LockedRot.Pitch = 0.f;
	LockedRot.Roll = 0.f;
}