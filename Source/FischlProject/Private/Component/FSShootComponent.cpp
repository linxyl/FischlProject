// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/FSShootComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Actor/FSCharacter.h"
#include "Projectile/FSProjectileBase.h"
#include "Component/FSBuffComponent.h"
#include "Actor/FSFischl.h"
#include "Actor/FSOz.h"

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


void UFSShootComponent::Shoot(TSubclassOf<AFSProjectileBase> ShootProjectileClass, int32 TempSerialNumber)
{
	if (Cast<AFSCharacter>(GetOwner())->GetBuffComp()->FindAndUseBuff("TripleShoot"))
	{
		LeftSerialNum = 3;
	}
	else if (-1 == TempSerialNumber)
	{
		LeftSerialNum = SerialNumber;
	}
	else
	{
		LeftSerialNum = TempSerialNumber;
	}

	TSubclassOf<AFSProjectileBase> TempProjectileClass = 
		ShootProjectileClass == TSubclassOf<AFSProjectileBase>() ? ProjectileClass[ProjectileSelect] : ShootProjectileClass;

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "ExecShoot", TempProjectileClass);
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, Delegate, Interval, true, 0.f);
	//GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UFSShootComponent::ShootImpl, Interval, true, 0.f);
}

void UFSShootComponent::ExecShoot_Implementation(TSubclassOf<AFSProjectileBase> ShootProjectileClass)
{
	if (ensure(ProjectileClass.Num() > ProjectileSelect))
	{
		AFSCharacter* Instigator = Cast<AFSCharacter>(GetOwner());

		FVector Location = Instigator->GetMesh()->GetSocketLocation(SocketName);
		FRotator Rot;
		if (Instigator->bIsLocked && Instigator->LockedActor)
			Rot = UKismetMathLibrary::FindLookAtRotation(Location, Instigator->LockedActor->GetActorLocation());
		else
			Rot = Instigator->GetActorRotation();
		Rot.Yaw += (ParallelNumber + 1) * IncludedAngle / 2;

		for (int32 i = 0; i < ParallelNumber; i++)
		{
			Rot.Yaw -= IncludedAngle;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = Instigator;

			FTransform SpawnTM = FTransform(Rot, Location);
			Cast<AFSProjectileBase>(GetWorld()->SpawnActor<AActor>(ShootProjectileClass, SpawnTM, SpawnParams));
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);

		if (Cast<AFSCharacter>(GetOwner())->GetBuffComp()->FindBuff("Collaborate"))
		{
			if (AFSFischl* Fischl = Cast<AFSFischl>(GetOwner()))
			{
				Fischl->GetOz()->GetShootComp()->Shoot(TSubclassOf<AFSProjectileBase>());
			}
		}

		if (0 == --LeftSerialNum)
		{
			//bFirstShoot = true;
			//TempProjectileClass = TSubclassOf<AFSProjectileBase>();
			GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
}