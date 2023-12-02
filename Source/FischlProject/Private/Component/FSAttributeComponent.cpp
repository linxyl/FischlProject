// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FSAttributeComponent.h"


UFSAttributeComponent::UFSAttributeComponent()
{
	SetMaxVal(100.0f, 100.0f, 100.0f);
}

void UFSAttributeComponent::SetMaxVal(float HPMax, float MPMax, float APMax)
{
	HealthMax = HPMax;
	Health = HPMax;
	ManaMax = MPMax;
	Mana = MPMax;
	AttentionMax = APMax;
	Attention = APMax;
}
