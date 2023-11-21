// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API UGEEC_Weapon : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UGEEC_Weapon();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
