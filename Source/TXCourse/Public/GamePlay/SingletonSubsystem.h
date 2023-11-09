// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Box/RandomAwardBoxFactory.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Weapon/WeaponFactory.h"
#include "SingletonSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API USingletonSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable,Category=Singleton)
	static USingletonSubsystem* GetInstance();

	UFUNCTION(BlueprintCallable,Category=Singleton)
	UWeaponFactory* GetWeaponFactoryInst();

	UFUNCTION(BlueprintCallable,Category=Singleton)
	URandomAwardBoxFactory* GetRandomAwardBoxFactoryInst();

private:

	UWeaponFactory* WeaponFactory;

	URandomAwardBoxFactory* RandomAwardBoxFactory;
	
};
