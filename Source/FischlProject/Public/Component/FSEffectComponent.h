// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "FSEffectComponent.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISCHLPROJECT_API UFSEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSEffectComponent();

public:
	/** Trigger when injured. */
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* InjuredVFX;
};
