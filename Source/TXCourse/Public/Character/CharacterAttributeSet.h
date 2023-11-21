// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TXCOURSE_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,MaxHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,Energy);

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,MaxEnergy);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,Armor);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_AttackLucky)
	FGameplayAttributeData AttackLucky;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,AttackLucky);

	UFUNCTION()
	virtual void OnRep_AttackLucky(const FGameplayAttributeData& OldAttackLucky);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing=OnRep_DefenceLucky)
	FGameplayAttributeData DefenceLucky;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet,DefenceLucky);

	UFUNCTION()
	virtual void OnRep_DefenceLucky(const FGameplayAttributeData& OldDefenceLucky);
};
