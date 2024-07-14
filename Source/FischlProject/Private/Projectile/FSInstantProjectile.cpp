// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FSInstantProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Actor/FSCharacter.h"

AFSInstantProjectile::AFSInstantProjectile()
{
	MovementComp->ProjectileGravityScale = 0.f;
	MovementComp->InitialSpeed = 0.f;
}

void AFSInstantProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!Target)
	{
		return;
	}

	float Pitch = FMath::FRandRange(-180.0f, 180.0f);
	float Yaw = FMath::FRandRange(-180.0f, 180.0f);
	float Roll = FMath::FRandRange(-180.0f, 180.0f);
	SetActorRotation(FRotator(Pitch, Yaw, Roll));
	SetActorLocation(Target->GetActorLocation());
}
