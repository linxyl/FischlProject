// Fill out your copyright notice in the Description page of Project Settings.


#include "FSAnimNotify_Move.h"
#include "Actor/FSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UFSAnimNotify_Move::UFSAnimNotify_Move()
{

}

void UFSAnimNotify_Move::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		Instigator = Cast<AFSCharacter>(MeshComp->GetOwner());

		if (Instigator)
		{
			Instigator->bAnimNotifyStateEnd = false;
		}
	}
}

void UFSAnimNotify_Move::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (Instigator && !Instigator->bAnimNotifyStateEnd)
	{
		FVector Velocity;
		float Yaw = Instigator->GetActorRotation().Yaw;

		Velocity.X = UKismetMathLibrary::DegCos(Yaw) * ForwardVelocity;
		Velocity.Y = UKismetMathLibrary::DegSin(Yaw) * ForwardVelocity;
		Velocity.Z = UpVelocity;

		if (Instigator->GetCharacterMovement()->IsFalling())
		{
			Instigator->GetCharacterMovement()->Velocity = Velocity;
		}
		else
		{
			Instigator->LaunchCharacter(Velocity, true, true);
		}
	}
}

void UFSAnimNotify_Move::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
