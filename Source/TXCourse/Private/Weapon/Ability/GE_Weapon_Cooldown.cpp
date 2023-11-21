// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability/GE_Weapon_Cooldown.h"

#include "Weapon/Ability/GMMC_Weapon_Cooldown.h"

UGE_Weapon_Cooldown::UGE_Weapon_Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FGameplayModifierInfo ModifierInfo;
	FCustomCalculationBasedFloat CustomCalculationBasedFloat;
	CustomCalculationBasedFloat.CalculationClassMagnitude = UGMMC_Weapon_Cooldown::StaticClass();
	DurationMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Cooldown")));
}
