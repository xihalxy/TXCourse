// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Box/RandomAwardBoxFactory.h"
#include "Shop/Shop.h"
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

	virtual ~USingletonSubsystem() override;
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable,Category=Singleton)
	static USingletonSubsystem* GetInstance();

	UFUNCTION(BlueprintCallable,Category=Singleton)
	UWeaponFactory* GetWeaponFactoryInst();

	UFUNCTION(BlueprintCallable,Category=Singleton)
	URandomAwardBoxFactory* GetRandomAwardBoxFactoryInst();

	UFUNCTION(BlueprintCallable,Category=Singleton)
	UShop* GetShopInst();

private:

	static UWeaponFactory* WeaponFactory;

	static URandomAwardBoxFactory* RandomAwardBoxFactory;

	static UShop* Shop;
	
};
