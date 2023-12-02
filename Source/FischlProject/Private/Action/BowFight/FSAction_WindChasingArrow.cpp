// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/BowFight/FSAction_WindChasingArrow.h"
#include "Actor/FSFischl.h"
#include "Projectile/FSProjectileBase.h"

UFSAction_WindChasingArrow::UFSAction_WindChasingArrow()
{
	ActionName = "WindChasingArrow";
}

void UFSAction_WindChasingArrow::StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg, float Arg)
{
	if (ensure(ProjectileClass))
	{
		Super::StartAction_Implementation(Instigator, bHasArg, Arg);

		FVector Location = Instigator->GetActorLocation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Instigator;

		FRotator Rot = Cast<AFSFischl>(Instigator)->LockedRot;
		Rot.Roll = 0.0f;
		Rot.Pitch = 0.0f;

		FTransform SpawnTM = FTransform(Rot, Location);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}
