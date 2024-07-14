// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSActorBase.h"
#include "GameFramework/Character.h"
#include "FSFunctionLibrary.h"
#include "FSCharacter.generated.h"

class UFSAttributeComponent;
class UFSActionComponent;
class UFSBuffComponent;
class UFSEffectComponent;
class UFSShootComponent;
class UFSWeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceivedDamage, AActor*, InstigatorActor, FVector, ImpactPoint, FDamageParam, DamageParam);

UCLASS(Blueprintable)
class FISCHLPROJECT_API AFSCharacter : public ACharacter, public FSActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSCharacter();

private:
	/** The AttributeComp storing attribute. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSAttributeComponent* AttributeComp;

	/** The ActionComp being used to manage action. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSActionComponent* ActionComp;

	/** The BuffComp being used to manage buff. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSBuffComponent* BuffComp;

	/** The EffectComp being used to manage effect. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSEffectComponent* EffectComp;

	/** The ShootComp being used to shoot projectile. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFSShootComponent* ShootComp;

protected:
	/** Weapon handled by the character. */
	UFSWeaponComponent* WeaponComponent;

public:
	/** Returns AttributeComp subobject **/
	FORCEINLINE class UFSAttributeComponent* GetAttributeComp() const { return AttributeComp; }

	/** Returns ActionComp subobject **/
	FORCEINLINE class UFSActionComponent* GetActionComp() const { return ActionComp; }

	/** Returns BuffComp subobject **/
	FORCEINLINE class UFSBuffComponent* GetBuffComp() const { return BuffComp; }

	/** Returns EffectComp subobject **/
	FORCEINLINE class UFSEffectComponent* GetEffectComp() const { return EffectComp; }

	/** Returns ShootComp subobject **/
	FORCEINLINE class UFSShootComponent* GetShootComp() const { return ShootComp; }

protected:
	/** Set Rotation interpolation between actor rotation and LockedRot. */
	UFUNCTION(BlueprintCallable)
	void RotateByLocked(float MaxRotYaw = 3.0f);

public:
	/** Whether is locked */
	UPROPERTY(BlueprintReadOnly)
	bool bIsLocked;

	/** If not null, it is the actor being locked, else express character is locking in one direction. */
	UPROPERTY(BlueprintReadOnly)
	APawn* LockedActor;

	/** Target lock direction. */
	UPROPERTY(BlueprintReadOnly)
	FRotator LockedRot;

	/** Offset of the bottom of character. */
	UPROPERTY(BlueprintReadOnly)
	FVector RootOffset;

public:
	/** Get the location of the bottom of character. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetRootLoaction() const { return GetActorLocation() + RootOffset; }

	/** Set whether the weapon is visible. */
	virtual void SetWeaponVisible(bool b = true);

	/** Set bOrientRotationToMovement according to the actor's status and input. */
	virtual bool UpdateOrientToMovement(bool bVal);

	/** Flag that indicate the end of AnimNotifyState. */
	UPROPERTY(BlueprintReadWrite)
	bool bAnimNotifyStateEnd;

	/** Classes ignored when attacking. */
	TArray<AActor*> IgnoredActors;

	/** Delegate that for receiving damage. */
	FOnReceivedDamage OnReceivedDamage;

//public:
//	UPROPERTY(BlueprintReadWrite)
//	bool bStopAnimNotify;

protected:
	/** Called when receiving damage. */
	UFUNCTION(BlueprintCallable)
	void ReceivedDamage(AActor* InstigatorActor, FVector ImpactPoint, FDamageParam DamageParam);

protected:
	//~ Begin AActor Interface.
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

	//~ Begin ACharacter Interface.
	virtual void Landed(const FHitResult& Hit) override;
	//~ End ACharacter Interface

private:
	/** Calculate locked rotation. */
	void UpdateLockedRot();
};
