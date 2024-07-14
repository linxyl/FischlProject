// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSCameraComponent();

public:
	/** Scale of Lens distance target */
	float ScrollScale;

protected:
	/** Used to adjust the position after the camera is blocked */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	/** Camera that captures a scene. */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

private:
	float DesiredTargetArmLength;

	FVector DesiredTargetOffset;

	FVector OffsetVelocity;

public:
	//~ Begin UActorComponent Interface.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.
};
