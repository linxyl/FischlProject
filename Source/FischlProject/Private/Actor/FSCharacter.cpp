// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSCharacter.h"
#include "Component/FSAttributeComponent.h"
#include "Component/FSActionComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "FSFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Typedef.h"
#include "Component/FSWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFSCharacter::AFSCharacter()
{
	// Set Mesh
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName("Body");
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation_Direct(FRotator(0.f, -90.f, 0.f));

	// Init attribute component
	AttributeComp = CreateDefaultSubobject<UFSAttributeComponent>("AttributeComp");
	AttributeComp->SetMaxVal(100.0f, 100.0f, 100.0f);

	// Init action component
	ActionComp = CreateDefaultSubobject<UFSActionComponent>("ActionComp");

	// Set Effect
	UObject* loadVFXObj = StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, TEXT("NiagaraSystem'/Game/Effects/FX_HunmanInjured.FX_HunmanInjured'"));
	if (loadVFXObj)
	{
		InjuredVFX = Cast<UNiagaraSystem>(loadVFXObj);
	}
}

// Called when the game starts or when spawned
void AFSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFSCharacter::ReceivedDamage(AActor* InstigatorActor, FVector ImpactPoint, FDamageParam DamageParam)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InjuredVFX, ImpactPoint, DamageParam.Direction.GetInverse(), FVector(1.f), true, true, ENCPoolMethod::None, true);
}

void AFSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnReceivedDamage.AddDynamic(this, &AFSCharacter::ReceivedDamage);
}

void AFSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	AFSCharacter* Character = Cast<AFSCharacter>(Hit.Actor);
	if (!Character)
	{
		ActionComp->StopCurrentAction(this);
		StopAnimMontage(GetCurrentMontage());

		GetCharacterMovement()->GravityScale = DEFAULT_GRAVITY;

		bIsLaunching = false;
	}
}

bool AFSCharacter::SetOrientToMovement(bool bVal)
{
	return GetCharacterMovement()->bOrientRotationToMovement = bVal && !bIsLocked;
}

FORCEINLINE FVector AFSCharacter::GetRootLoaction() const
{
	return GetActorLocation() + RootOffset;
}

void AFSCharacter::SetWeaponVisible(bool b /*= true*/)
{
	Weapon->SetVisibility(true);
}

// Called every frame
void AFSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling() && GetCharacterMovement()->GravityScale < FAST_GRAVITY)
	{
		GetCharacterMovement()->GravityScale += DeltaTime / 15;
	}

	//防止跳跃时撞到墙而使水平速度消失
	//if (bIsLaunching)
	//{
	//	GetCharacterMovement()->Velocity = FVector(VelocityXY.X, VelocityXY.Y, GetVelocity().Z);
	//}
}

// Called to bind functionality to input
void AFSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

