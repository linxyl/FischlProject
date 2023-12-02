// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/FSCharacter.h"
#include "FSEnemy.generated.h"

UCLASS()
class FISCHLPROJECT_API AFSEnemy : public AFSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
