// Fill out your copyright notice in the Description page of Project Settings.

#include "FSFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "Actor/FSCharacter.h"
#include "Typedef.h"

bool UFSFunctionLibrary::ApplyFuncDamage(AActor* DamageCauser, AActor* TargetActor, UPARAM(ref) TArray<AActor*>& DamagedActor, const FVector ImpactPoint, const FDamageParam& DamageParam)
{
	ACharacter* Target = Cast<ACharacter>(TargetActor);
	
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamege Error!"));
		return false;
	}

	if (DamagedActor.Contains(TargetActor))
	{
		return false;
	}
	DamagedActor.Add(TargetActor);


	static TMap<ACharacter*, FTimerHandle> TimerMap;
	FTimerHandle* pHandle = TimerMap.Find(Target);
	if (pHandle)
	{
		DamageCauser->GetWorld()->GetTimerManager().ClearTimer(*pHandle);
		TimerMap.Remove(Target);

		TargetActor->CustomTimeDilation = 1.f;
	}

	if (!ensure(&DamageParam))
	{
		return false;
	}

	TargetActor->CustomTimeDilation = DamageParam.Dilation;

	FTimerHandle& TimerHandle = TimerMap.Add(Target, FTimerHandle());
	DamageCauser->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Target]()
		{
			if (Target)
			{
				Target->CustomTimeDilation = 1;
			}
			TimerMap.Remove(Target);
		},
		DamageParam.DilTime, false);


	if (Target->GetMovementComponent()->IsFalling() || DamageParam.bEnbleFly)
	{
		//if (!Target->GetMovementComponent()->IsFalling() && DamageParam.bEnbleFly)
		//{
			Target->GetCharacterMovement()->GravityScale = UFSFunctionLibrary::SlowGravity();
		//}

		FVector Direction = TargetActor->GetActorLocation() - DamageCauser->GetActorLocation();
		float Temp;
		Direction.Z = 0.0f;
		Direction.ToDirectionAndLength(Direction, Temp);

		Direction.X *= DamageParam.ForwardVelocity;
		Direction.Y *= DamageParam.ForwardVelocity;
		Direction.Z = DamageParam.UpVelocity;

		Target->GetMovementComponent()->Velocity = { 0.0f, 0.0f, 0.0f };
		Target->LaunchCharacter(Direction, true, true);
	}

	Cast<AFSCharacter>(TargetActor)->OnReceivedDamage.Broadcast(DamageCauser, ImpactPoint, DamageParam);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(DamageCauser->GetWorld(), DamageParam.ImpactVFX, ImpactPoint, DamageParam.Direction, FVector(1.f), true, true, ENCPoolMethod::None, true);
	
	UGameplayStatics::PlaySoundAtLocation(DamageCauser->GetWorld(), DamageParam.Sound, TargetActor->GetActorLocation());

	return true;
}

//UNiagaraComponent* UFSFunctionLibrary::CustomSpawnSystemAtLocation(AActor* Instigator, UNiagaraSystem* SystemTemplate, FVector SpawnLocation, FRotator SpawnRotation, FVector Scale)
//{
//	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(Instigator->GetWorld(), SystemTemplate, SpawnLocation, SpawnRotation, Scale, true, true, ENCPoolMethod::None, true);
//}
