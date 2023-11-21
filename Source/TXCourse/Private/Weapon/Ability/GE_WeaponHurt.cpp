// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability/GE_WeaponHurt.h"
#include "Weapon/Ability/GEEC_Weapon.h"

UGE_WeaponHurt::UGE_WeaponHurt()
{
	FGameplayEffectExecutionDefinition GEED ;
	GEED.CalculationClass = UGEEC_Weapon::StaticClass();
	Executions.Add(GEED);
}
