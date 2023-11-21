// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Ability//GEEC_Weapon.h"

#include "Character/CharacterAttributeSet.h"
#include "Weapon/Ability/WeaponAttributeSet.h"



UGEEC_Weapon::UGEEC_Weapon()
{
	
}

void UGEEC_Weapon::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UWeaponAttributeSet* AS_Source = Cast<UWeaponAttributeSet>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAttributeSet(UWeaponAttributeSet::StaticClass()));
	const UCharacterAttributeSet* AS_Target = Cast<UCharacterAttributeSet>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAttributeSet(UCharacterAttributeSet::StaticClass()));

	
	float Armor = AS_Target->GetArmor();
	float DefenceLucky = AS_Target->GetDefenceLucky();
	float DamageDecrease = FMath::RandRange(0.f,100.f)<DefenceLucky ? 2.0f : 1.0f;
	float Damage = AS_Source->GetDamage();
	float AttackLucky = AS_Source->GetAttackLucky();
	float DamageIncrease = FMath::RandRange(0.f,100.f)<AttackLucky ? 2.0f : 1.0f;
	float FinalDamage = (DamageIncrease*Damage-Armor)/DamageDecrease;
	if(FinalDamage>0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(AS_Target->GetHealthAttribute(),EGameplayModOp::Additive,-FinalDamage)
		);
	}
}
