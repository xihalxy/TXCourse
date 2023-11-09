// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SingletonSubsystem.h"

void USingletonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USingletonSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

USingletonSubsystem* USingletonSubsystem::GetInstance()
{
	return GWorld->GetGameInstance()->GetSubsystem<USingletonSubsystem>();
}

UWeaponFactory* USingletonSubsystem::GetWeaponFactoryInst()
{
	if(WeaponFactory==nullptr)
	{
		WeaponFactory = NewObject<UWeaponFactory>();
	}
	return WeaponFactory;
}

URandomAwardBoxFactory* USingletonSubsystem::GetRandomAwardBoxFactoryInst()
{
	if(RandomAwardBoxFactory==nullptr)
	{
		RandomAwardBoxFactory = NewObject<URandomAwardBoxFactory>();
	}
	return RandomAwardBoxFactory;
}
