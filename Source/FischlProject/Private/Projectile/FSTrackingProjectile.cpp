// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FSTrackingProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFSTrackingProjectile::AFSTrackingProjectile():
	MaxAngularVelocity(60.f)
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComp->ProjectileGravityScale = 0.f;
	MovementComp->bRotationFollowsVelocity = false;
}

void AFSTrackingProjectile::BeginPlay()
{
	Super::BeginPlay();

	//MovementComp->Velocity = FVector(0.f);
}

void AFSTrackingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target)
	{
		return;
	}

	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	FRotator CurRot = GetActorRotation();
	FRotator DiffRot = (TargetRot - CurRot).Clamp();

	float Pitch = DiffRot.Pitch;
	float Yaw = DiffRot.Yaw;

	if (Pitch < 180.f && Pitch > MaxAngularVelocity * DeltaTime)
	{
		Pitch = MaxAngularVelocity * DeltaTime;
	}
	else if (Pitch > 180.f && Pitch - 360.f < -MaxAngularVelocity * DeltaTime)
	{
		Pitch = -MaxAngularVelocity * DeltaTime;
	}

	if (Yaw < 180.f && Yaw > MaxAngularVelocity * DeltaTime)
	{
		Yaw = MaxAngularVelocity * DeltaTime;
	}
	else if (Yaw < 180.f && Yaw - 360.f < -MaxAngularVelocity * DeltaTime)
	{
		Yaw = -MaxAngularVelocity * DeltaTime;
	}

	AddActorLocalRotation(FRotator(Pitch, Yaw, 0.f));
	CurRot = GetActorRotation();
	AddActorWorldOffset(DeltaTime * MovementComp->InitialSpeed * CurRot.Vector());
}
