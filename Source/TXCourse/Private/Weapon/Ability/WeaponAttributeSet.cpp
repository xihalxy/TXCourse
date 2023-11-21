// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability//WeaponAttributeSet.h"
#include "Net/UnrealNetwork.h"

UWeaponAttributeSet::UWeaponAttributeSet()
{
}

void UWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponAttributeSet,Damage);
	DOREPLIFETIME(UWeaponAttributeSet,AttackLucky);
	DOREPLIFETIME(UWeaponAttributeSet,CoolDown);
	DOREPLIFETIME(UWeaponAttributeSet,Cost);
}

void UWeaponAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet,Damage,OldDamage);
}

void UWeaponAttributeSet::OnRep_AttackLucky(const FGameplayAttributeData& OldAttackLucky)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet,AttackLucky,OldAttackLucky);
}

void UWeaponAttributeSet::OnRep_CoolDown(const FGameplayAttributeData& OldCoolDown)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet,CoolDown,OldCoolDown);
}

void UWeaponAttributeSet::OnRep_Cost(const FGameplayAttributeData& OldCost)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet,Cost,OldCost);
}

