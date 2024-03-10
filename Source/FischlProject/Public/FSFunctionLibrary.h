// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Typedef.h"
#include "Engine/DataTable.h"
#include "FSFunctionLibrary.generated.h"

class USoundBase;
class UNiagaraSystem;
class UNiagaraComponent;
class AFSCharacter;

USTRUCT(BlueprintType)
struct FDamageParam : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float DamageAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float ForwardVelocity = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float UpVelocity = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	FRotator Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	bool bEnbleFly = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float Dilation = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	float DilTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property")
	UNiagaraSystem* ImpactVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	USoundBase* Sound = nullptr;
};

/**
 * 
 */
UCLASS()
class FISCHLPROJECT_API UFSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Custom")
	static bool ApplyFuncDamage(AActor* DamageCauser, AActor* TargetActor, UPARAM(ref) TArray<AActor*>& DamagedActorTArray, const FVector ImpactPoint, const FDamageParam& DamageParam);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float DefaultGravity(){ return DEFAULT_GRAVITY; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float FastGravity() { return FAST_GRAVITY; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float SlowGravity() { return SLOW_GRAVITY; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ConstValue")
	static float NoGravity() { return NO_GRAVITY; }

	//UFUNCTION(BlueprintCallable, Category = "Custom")
	//static UNiagaraComponent* CustomSpawnSystemAtLocation(AActor* Instigator, UNiagaraSystem* SystemTemplate, FVector SpawnLocation, FRotator SpawnRotation, FVector Scale);
};
