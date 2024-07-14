// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FSAnimNotifyState_Collision.h"
#include "FSFunctionLibrary.h"
#include "Actor/FSCharacter.h"
#include "Component/FSActionComponent.h"
#include "Action/FSAction.h"
#include "DrawDebugHelpers.h"
#include "Engine/DataTable.h"

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
			Action = Instigator->GetActionComp()->FindAction(ActionName);
			ensure(Action);
		}
	}

	if (DataTable)
	{
		uint8* const* RowDataPtr = DataTable->GetRowMap().Find(RowName);
		if (RowDataPtr)
		{
			DamageParam = reinterpret_cast<FDamageParam*>(*RowDataPtr);
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

	FVector SockLoc = MeshComp->GetSocketLocation(Socket);
	FRotator SocketRot = MeshComp->GetSocketRotation(Socket);

	FVector PosStart = SockLoc + SocketRot.RotateVector(OffsetStart);
	FVector PosEnd = SockLoc + SocketRot.RotateVector(OffsetEnd);

	TArray<FHitResult> AllResults;
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
		if (Res.Actor != Instigator && Res.Actor != Instigator->GetInstigator())
		{
			UFSFunctionLibrary::ApplyFuncDamage(Instigator, Res.Actor.Get(), Action->DamagedActors, Res.ImpactPoint, *DamageParam);
			Action->CustomEvent(Instigator, Cast<AFSCharacter>(Res.Actor.Get()), 0.f);

			if (bOnce)
			{
				OnceFlag = true;
				return;
			}
		}
	}
}

void UFSAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (Action)
	{
		Action->DamagedActors.Empty();
	}
}
