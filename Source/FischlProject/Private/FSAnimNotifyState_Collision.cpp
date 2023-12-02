// Fill out your copyright notice in the Description page of Project Settings.


#include "FSAnimNotifyState_Collision.h"
#include "FSFunctionLibrary.h"
#include "Actor/FSCharacter.h"
#include "Component/FSActionComponent.h"
#include "Action/FSAction.h"

UFSAnimNotifyState_Collision::UFSAnimNotifyState_Collision()
{
	CollisionRadius = 15.f;
}

void UFSAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());
		if (Instigator)
		{
			Action = Instigator->ActionComp->FindAction(ActionName);
			ensure(Action);
		}
	}

	OnceFlag = false;

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UFSAnimNotifyState_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (OnceFlag || !Instigator)
	{
		return;
	}

	FVector PosStart = MeshComp->GetSocketLocation(SocketStart);
	FVector PosEnd = MeshComp->GetSocketLocation(SocketEnd);

	Instigator->GetWorld()->SweepMultiByProfile(
		AllResults,
		PosStart,
		PosEnd,
		FQuat::Identity,
		CollisionProfile,
		FCollisionShape::MakeSphere(CollisionRadius),
		FCollisionQueryParams::FCollisionQueryParams(false)
	);

	for (auto& Res : AllResults)
	{
		if (Res.Actor != Instigator)
		{
			UFSFunctionLibrary::ApplyFuncDamage(Instigator, Res.Actor.Get(), Action->DamagedActors, Res.ImpactPoint, Action->DamageParam);
			Action->CustomEvent(Instigator, Cast<AFSCharacter>(Res.Actor.Get()), 0.f);

			if (bOnce)
			{
				OnceFlag = true;
				return;
			}
		}
	}
}
