// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSOz.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"

#include "Actor/FSFischl.h"
#include "Component/FSBuffComponent.h"
#include "Component/FSActionComponent.h"

AFSOz::AFSOz()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	GetCharacterMovement()->GravityScale = 0.f;
}

void AFSOz::Appear()
{
	GetMesh()->SetVisibility(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	DisplayFX();
}

void AFSOz::Hide()
{
	GetMesh()->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DisplayFX();
}

void AFSOz::PostAction()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetActionComp()->StopCurrentAction(this);
	Hide();
}

void AFSOz::BeginPlay()
{
	Super::BeginPlay();

	Fischl = Cast<AFSFischl>(GetInstigator());

	StealthVFXComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		StealthVFX,
		GetMesh(),
		"BeakSocket",
		{ 0.f,0.f,0.f },
		FRotator(),
		EAttachLocation::SnapToTargetIncludingScale,
		false
	);

	IgnoredActors.Add(Fischl);

	Hide();

	DisplayFX();
}

void AFSOz::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ensure(Fischl))
	{
		bIsLocked = Fischl->bIsLocked;
		LockedActor = Fischl->LockedActor;
	}

	StealthVFXComp->SetWorldRotation(FRotator(0.f, 90.f, 0.f));

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	if (GetMesh()->IsVisible())
	{
		if (nullptr == GetActionComp()->GetCurrentAction())
		{
			RotateByLocked();
		}
	}
	else
	{
		SetActorLocation(FMath::Lerp(GetTargetStealthLocation(), GetActorLocation(), 0.9));
	}
}

void AFSOz::DisplayFX()
{
	if (!GetMesh()->GetVisibleFlag() && Fischl->GetBuffComp()->FindBuff("Collaborate"))
	{
		if (!StealthVFXComp->IsActive())
		{
			StealthVFXComp->Activate();
		}
	}
	else
	{
		StealthVFXComp->DeactivateImmediate();
	}
}

FVector AFSOz::GetTargetStealthLocation() const
{
	FVector Offset = 50 * UKismetMathLibrary::GetRightVector(Fischl->GetActorRotation() + FRotator(0.f, 25.f, 0.f));
	Offset.Z = 40 + 5 * FMath::Cos(3.5 * GetWorld()->TimeSeconds);

	return (Fischl->GetActorLocation() + Offset);
}