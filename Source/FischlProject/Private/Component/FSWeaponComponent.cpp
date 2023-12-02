// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSWeaponComponent.h"
#include "DrawDebugHelpers.h"
#include "Actor/FSCharacter.h"
#include "FSFunctionLibrary.h"
#include "NiagaraSystem.h"

UFSWeaponComponent::UFSWeaponComponent()
{
	SetCollisionProfileName("Attack");
	SetGenerateOverlapEvents(false);

	UObject* LoadVFXObj = StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, TEXT("NiagaraSystem'/Game/Effects/FX_ImpactLit.FX_ImpactLit'"));
	if (LoadVFXObj)
	{
		DamageParam.ImpactVFX = Cast<UNiagaraSystem>(LoadVFXObj);
	}
}

void UFSWeaponComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (OtherActor)
	{
		TArray<FHitResult> AllResults;

		FVector Start = OverlappedComponent->GetComponentLocation();
		FVector End = OtherComp->GetComponentLocation();

		// Use a slightly larger radius to ensure we find the same result
		float CollisionRadius = FVector::Dist(Start, End) * 1.1f;

		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);

		// Now do a spherical sweep to find the overlap
		GetWorld()->SweepMultiByObjectType(
			AllResults,
			Start,
			End,
			FQuat::Identity,
			CollisionObjectQueryParams,
			FCollisionShape::MakeSphere(CollisionRadius),
			FCollisionQueryParams::FCollisionQueryParams(false)
		);

		for (const auto& HitResult : AllResults)
		{
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 1, 32, FColor(255,0,0), true, 0.0f);
			if (OtherComp->GetUniqueID() == HitResult.GetComponent()->GetUniqueID())
			{
				UFSFunctionLibrary::ApplyFuncDamage(GetOwner(), OtherActor, DamagedActors, HitResult.ImpactPoint, DamageParam);
			}
		}
	}
}

void UFSWeaponComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnComponentBeginOverlap.AddDynamic(this, &UFSWeaponComponent::BeginOverlap);
}
