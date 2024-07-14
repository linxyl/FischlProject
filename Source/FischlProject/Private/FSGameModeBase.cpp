// Fill out your copyright notice in the Description page of Project Settings.


#include "FSGameModeBase.h"
#include "Component/FSBuffComponent.h"
#include "Buff/FSBuffBase.h"

void AFSGameModeBase::StartPlay()
{
	Super::StartPlay();

	for (const auto& Buff : BuffClasses)
	{
		if (Buff == TSubclassOf<UFSBuffBase>())
		{
			continue;
		}

		TArray<FString> Substrings1, Substrings2;
		Buff->GetName().ParseIntoArray(Substrings1, TEXT("BP_Buff"), true);
		Substrings1[0].ParseIntoArray(Substrings2, TEXT("_C"), true);

		BuffClassMap.Add(FName(Substrings2[0]), Buff);
	}
}

FORCEINLINE TSubclassOf<UFSBuffBase> AFSGameModeBase::FindBuffClass(FName BuffName)
{
 	return *(BuffClassMap.Find(BuffName));
}
