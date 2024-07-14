// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FSProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Actor/FSCharacter.h"
#include "Component/FSBuffComponent.h"
#include "Actor/FSFischl.h"
#include "Actor/FSOz.h"
#include "Component/FSShootComponent.h"

// Sets default values
AFSProjectileBase::AFSProjectileBase():
	bDamageOnce(true)
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.1f;
	MovementComp->InitialSpeed = 3000;
}

void AFSProjectileBase::Explode(AActor* ExplodeTarget)
{
	UFSFunctionLibrary::ApplyFuncDamage(GetInstigator(), ExplodeTarget, IgnoredActors, GetActorLocation(), Param);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeVFX, GetActorLocation(), GetActorRotation(), FVector(1.f), true, true, ENCPoolMethod::None, true);

	if (bDamageOnce)
	{
		Destroy();
	}
}

void AFSProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFSProjectileBase::OnOverlap);
}

void AFSProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<AFSCharacter>(GetInstigator())->LockedActor;

	SetLifeSpan(60.f);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnVFX, GetActorLocation(), GetActorRotation(), FVector(1.f), true, true, ENCPoolMethod::None, true);

	IgnoredActors.Add(GetInstigator());
}

void AFSProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto it : IgnoredActors)
	{
		if (it == OtherActor)
		{
			return;
		}
	}

	for (auto it : Cast<AFSCharacter>(GetInstigator())->IgnoredActors)
	{
		if (it == OtherActor)
		{
			return;
		}
	}

	Explode(OtherActor);
}
