// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSShootComponent.h"
#include "Actor/FSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile/FSProjectileBase.h"

// Sets default values for this component's properties
UFSShootComponent::UFSShootComponent():
	SerialNumber(1),
	ParallelNumber(1),
	Interval(0.15f),
	IncludedAngle(15.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UFSShootComponent::Shoot()
{
	LeftSerialNum = SerialNumber;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UFSShootComponent::OnceShoot, Interval, true, 0.f);
}

void UFSShootComponent::OnceShoot()
{
	if (ensure(ProjectileClass))
	{
		AFSCharacter* Instigator = Cast<AFSCharacter>(GetOwner());

		FVector Location = Instigator->GetMesh()->GetSocketLocation(SocketName);
		FRotator Rot;
		if (Instigator->bIsLocked && Instigator->LockedActor)
			Rot = UKismetMathLibrary::FindLookAtRotation(Location, Instigator->LockedActor->GetActorLocation());
		else
			Rot = Instigator->GetActorRotation();
		Rot.Yaw += (ParallelNumber + 1) * IncludedAngle / 2;

		for (uint32 i = 0; i < ParallelNumber; i++)
		{
			Rot.Yaw -= IncludedAngle;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = Instigator;

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShootVFX, Location, Rot, FVector(1.f), true, true, ENCPoolMethod::None, true);

			FTransform SpawnTM = FTransform(Rot, Location);
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		}

		if (0 == --LeftSerialNum)
		{
			GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
}