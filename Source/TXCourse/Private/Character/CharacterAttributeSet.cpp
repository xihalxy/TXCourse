// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet,Energy,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet,MaxEnergy,COND_None,REPNOTIFY_Always);
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,Health,OldHealth);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,MaxHealth,OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,Energy,OldEnergy);
}

void UCharacterAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,MaxEnergy,OldMaxEnergy);
}

void UCharacterAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,Armor,OldArmor);
}

void UCharacterAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,Attack,OldAttack);
}

void UCharacterAttributeSet::OnRep_AttackLucky(const FGameplayAttributeData& OldAttackLucky)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,AttackLucky,OldAttackLucky);
}

void UCharacterAttributeSet::OnRep_DefenceLucky(const FGameplayAttributeData& OldDefenceLucky)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet,DefenceLucky,OldDefenceLucky);
}
