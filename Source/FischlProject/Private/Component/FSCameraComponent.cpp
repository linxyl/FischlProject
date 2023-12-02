// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Actor/FSCharacter.h"
#include "Typedef.h"

// Sets default values for this component's properties
UFSCameraComponent::UFSCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ScrollScale = 1.f;
	DesiredTargetArmLength = SPRINGARM_LEN;

	// Init sprint arm component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetUsingAbsoluteRotation(true);
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 40));
	SpringArmComp->SetupAttachment(this);

	// Init camera component
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called every frame
void UFSCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AFSCharacter* ChaOwner = Cast<AFSCharacter>(GetOwner());
	if (!ChaOwner)
		return;

	if (ChaOwner->bIsLocked)
	{
		if (ChaOwner->LockedActor)
		{
			//Set Camera Location
			FVector&& RotCenter = (ChaOwner->GetActorLocation() + ChaOwner->LockedActor->GetActorLocation()) / 2;

			DesiredTargetOffset = RotCenter - ChaOwner->GetActorLocation();
			OffsetVelocity = (DesiredTargetOffset - SpringArmComp->TargetOffset) * 0.1f;
			SpringArmComp->TargetOffset += OffsetVelocity;

			DesiredTargetArmLength = ScrollScale * (ChaOwner->GetActorLocation() - ChaOwner->LockedActor->GetActorLocation()).Size() * 0.6f + 250;
		}
	}
	else
	{
		DesiredTargetOffset = { 0, 0, 0 };
		OffsetVelocity = (DesiredTargetOffset - SpringArmComp->TargetOffset) * 0.1f;
		SpringArmComp->TargetOffset += OffsetVelocity;

		DesiredTargetArmLength = SPRINGARM_LEN * ScrollScale;
	}
	float ArmLengthVelocity = (DesiredTargetArmLength - SpringArmComp->TargetArmLength) * 0.1f;
	SpringArmComp->TargetArmLength += ArmLengthVelocity;
}

