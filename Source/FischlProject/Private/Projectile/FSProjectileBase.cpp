// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FSProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Actor/FSCharacter.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AFSProjectileBase::AFSProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.1f;
	MoveComp->InitialSpeed = 3000;
}

void AFSProjectileBase::Explode(AActor* Target)
{
	UFSFunctionLibrary::ApplyFuncDamage(GetInstigator(), Target, DamagedActors, GetActorLocation(), Param);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeVFX, GetActorLocation(), GetActorRotation(), FVector(1.f), true, true, ENCPoolMethod::None, true);

	Destroy();
}

void AFSProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFSProjectileBase::OnOverlap);
}

void AFSProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(1.f);

	DamagedActors.Add(GetInstigator());
}

void AFSProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto it : DamagedActors)
	{
		if (it == OtherActor)
		{
			return;
		}
	}

	Explode(OtherActor);
}
