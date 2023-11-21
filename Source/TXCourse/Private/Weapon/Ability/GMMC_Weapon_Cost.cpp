// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability/GMMC_Weapon_Cost.h"

#include "Character/PlayerCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/Ability/WeaponAttributeSet.h"

float UGMMC_Weapon_Cost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(Spec.GetEffectContext().GetInstigator());
	if(Character&&Character->weapon_r)
	{
		return -Character->weapon_r->AttributeSet->GetCost();
	}
	return 0;
}
