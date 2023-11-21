// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/DamageGEExecutionCalculation.h"


struct SDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackLucky);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenceLucky);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	SDamageStatics()
	{
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet,AttackLucky,Source,true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet,Armor,Target,true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet,Health,Target,true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet,DefenceLucky,Target,true);
	}
		
};
static const SDamageStatics& DamageStatics()
{
	static SDamageStatics DStatics;
	return DStatics;
}

UDamageGEExecutionCalculation::UDamageGEExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackLuckyDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenceLuckyDef);
}

void UDamageGEExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	FAggregatorEvaluateParameters EvaluateParameters;
	
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,Armor);

	float DefenceLucky = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceLuckyDef,EvaluateParameters,DefenceLucky);

	float DamageDecrease = FMath::RandRange(0.f,100.f)<DefenceLucky ? 2.0f : 1.0f;
	
	float Attack = 0.0f;

	float AttackLucky = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackLuckyDef,EvaluateParameters,AttackLucky);

	float DamageIncrease = FMath::RandRange(0.f,100.f)<AttackLucky ? 2.0f : 1.0f;
	
	float FinalDamage = (DamageIncrease*Attack-Armor)/DamageDecrease;
	
	if(FinalDamage>0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(DamageStatics().HealthProperty,EGameplayModOp::Additive,-FinalDamage)
		);
	}
}
