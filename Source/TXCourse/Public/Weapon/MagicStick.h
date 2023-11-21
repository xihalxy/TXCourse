// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "MagicStick.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API AMagicStick : public AWeaponBase
{
	GENERATED_BODY()

	AMagicStick();

	UFUNCTION(BlueprintCallable)
	virtual void StartAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void EndAttack() override;
	
};
