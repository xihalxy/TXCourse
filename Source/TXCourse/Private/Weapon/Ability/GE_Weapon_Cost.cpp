// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability/GE_Weapon_Cost.h"
#include "Weapon/Ability/GMMC_Weapon_Cost.h"
#include "Character/CharacterAttributeSet.h"

UGE_Weapon_Cost::UGE_Weapon_Cost()
{
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UCharacterAttributeSet::GetEnergyAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	FCustomCalculationBasedFloat CustomCalculationBasedFloat;
	CustomCalculationBasedFloat.CalculationClassMagnitude = UGMMC_Weapon_Cost::StaticClass();
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);
	Modifiers.Add(ModifierInfo);
}
