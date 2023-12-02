// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSActorBase.h"
#include "GameFramework/Character.h"
#include "FSFunctionLibrary.h"
#include "FSCharacter.generated.h"

class UFSAttributeComponent;
class UFSActionComponent;
class UNiagaraSystem;
class UFSWeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceivedDamage, AActor*, InstigatorActor, FVector, ImpactPoint, FDamageParam, DamageParam);

UCLASS(Blueprintable)
class FISCHLPROJECT_API AFSCharacter : public ACharacter, public FSActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetRootLoaction() const;

	virtual void SetWeaponVisible(bool b = true);

	virtual bool SetOrientToMovement(bool bVal);

	UPROPERTY(BlueprintReadWrite)
	bool bIsLaunching;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLocked;

	UPROPERTY(BlueprintReadWrite)
	AActor* LockedActor;

	UPROPERTY(BlueprintReadWrite)
	FRotator LockedRot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UFSActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly)
	FVector RootOffset;

	UPROPERTY(BlueprintReadWrite)
	bool bEnDoubleJump;

	FOnReceivedDamage OnReceivedDamage;

	UPROPERTY(BlueprintReadWrite)
	bool bAnimNotifyStateEnd;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Landed(const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UFSAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* InjuredVFX;

	UFUNCTION(BlueprintCallable)
	void ReceivedDamage(AActor* InstigatorActor, FVector ImpactPoint, FDamageParam DamageParam);

	UFSWeaponComponent* Weapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
