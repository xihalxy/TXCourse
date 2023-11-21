// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Character/CharacterAttributeSet.h"
#include "DamageGEExecutionCalculation.generated.h"

/**
 * 
 */

UCLASS()
class TXCOURSE_API UDamageGEExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UDamageGEExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
