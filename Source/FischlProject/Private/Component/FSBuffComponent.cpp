// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSBuffComponent.h"

#include "GameFramework/Character.h" 
#include "Kismet/GameplayStatics.h"
#include "Buff/FSBuffBase.h"
#include "FSGameModeBase.h"

// Sets default values for this component's properties
UFSBuffComponent::UFSBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFSBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	for(const auto& Buff : DefaultBuffs)
	{
		TArray<FString> Substrings1, Substrings2;
		Buff->GetName().ParseIntoArray(Substrings1, TEXT("BP_Buff"), true);
		Substrings1[0].ParseIntoArray(Substrings2, TEXT("_C"), true);

		UFSBuffBase* NewBuff = NewObject<UFSBuffBase>(GetOwner(), Buff.Get());
		NewBuff->Init(Cast<ACharacter>(GetOwner()));

		MapBuff.Add(FName(Substrings2[0]), NewBuff);
	}
}

void UFSBuffComponent::AddBuff(FName BuffName)
{
	AActor* Owner = GetOwner();
	if (ensure(Owner))
	{
		AFSGameModeBase* GameMode = Cast<AFSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UFSBuffBase* NewBuff = NewObject<UFSBuffBase>(GetOwner(), GameMode->FindBuffClass(BuffName));

		NewBuff->Init(Cast<ACharacter>(GetOwner()), true);

		MapBuff.Add(BuffName, NewBuff);
	}
}

FORCEINLINE void UFSBuffComponent::RemoveBuff(FName BuffName)
{
	MapBuff.Remove(BuffName);
}

FORCEINLINE UFSBuffBase* UFSBuffComponent::FindBuff(FName BuffName)
{
	if (auto Buff = MapBuff.Find(BuffName))
	{
		return *Buff;
	}
	return nullptr;
}

FORCEINLINE UFSBuffBase* UFSBuffComponent::FindAndUseBuff(FName BuffName)
{
	if (auto Buff = MapBuff.Find(BuffName))
	{
		auto& RemainingTimes = (*Buff)->RemainingTimes;
		if (RemainingTimes > 0)
		{
			if (0 == (--RemainingTimes))
			{
				MapBuff.Remove(BuffName);
			}
		}

		return *Buff;
	}
	return nullptr;
}