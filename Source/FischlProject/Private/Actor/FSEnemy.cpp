// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSEnemy.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AFSEnemy::AFSEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
}