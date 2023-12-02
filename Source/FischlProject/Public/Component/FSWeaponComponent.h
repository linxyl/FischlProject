// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSFunctionLibrary.h"
#include "FSWeaponComponent.generated.h"

class UNiagaraSystem;

/**
 *
 */
UCLASS()
class FISCHLPROJECT_API UFSWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UFSWeaponComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FDamageParam DamageParam;

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	TArray<AActor*> DamagedActors;

	virtual void InitializeComponent() override;
};
