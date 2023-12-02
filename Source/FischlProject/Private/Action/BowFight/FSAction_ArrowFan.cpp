// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/BowFight/FSAction_ArrowFan.h"
#include "Actor/FSFischl.h"
#include "Components/CapsuleComponent.h"

UFSAction_ArrowFan::UFSAction_ArrowFan()
{
	ActionName = "ArrowFan";
}

void UFSAction_ArrowFan::StartAction_Implementation(AFSCharacter* Instigator, bool bHasArg, float Arg)
{
	Super::StartAction_Implementation(Instigator, bHasArg, Arg);

	//TArray<AActor*> Arr;
	//Cast<AFSFischl>(Instigator)->JumpCheckComp->GetOverlappingActors(Arr, AActor::StaticClass());

	//for (auto& val : Arr) {
	//	if (val != Instigator)
	//	{
	//		//((ACharacter*)val)->LaunchCharacter(FVector(0, 0, 1900), true, true);
	//		FDamageParam DamageParam;
	//		DamageParam.ForwardVelocity = 1000;
	//		DamageParam.UpVelocity = 1000;
	//		DamageParam.bEnbleFly = true;
	//		UFSFunctionLibrary::ApplyFuncDamage(Instigator, val, DamageParam);
	//	}
	//}
}