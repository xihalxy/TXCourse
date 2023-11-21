// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Weapon/WeaponBase.h"
#include "NoneWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API ANoneWeapon : public AWeaponBase
{
	GENERATED_BODY()

	ANoneWeapon();
public:

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	
};
