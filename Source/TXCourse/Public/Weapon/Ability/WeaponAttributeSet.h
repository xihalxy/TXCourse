// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TXCOURSE_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UWeaponAttributeSet();

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet,Damage);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_AttackLucky)
	FGameplayAttributeData AttackLucky;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet,AttackLucky);

	UFUNCTION()
	virtual void OnRep_AttackLucky(const FGameplayAttributeData& OldAttackLucky);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_CoolDown)
	FGameplayAttributeData CoolDown;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet,CoolDown);

	UFUNCTION()
	virtual void OnRep_CoolDown(const FGameplayAttributeData& OldCoolDown);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_Cost)
	FGameplayAttributeData Cost;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet,Cost);

	UFUNCTION()
	virtual void OnRep_Cost(const FGameplayAttributeData& OldCost);
};
