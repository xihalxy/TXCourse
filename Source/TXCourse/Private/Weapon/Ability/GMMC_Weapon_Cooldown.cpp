// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability/GMMC_Weapon_Cooldown.h"

#include "Character/PlayerCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/Ability/WeaponAttributeSet.h"

float UGMMC_Weapon_Cooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if(APlayerCharacter* Character = Cast<APlayerCharacter>(Spec.GetEffectContext().GetInstigator()))
	{
		if(Character->weapon_r)
		{
			return Character->weapon_r->AttributeSet->GetCoolDown();
		}
	}
	return 0;
}
