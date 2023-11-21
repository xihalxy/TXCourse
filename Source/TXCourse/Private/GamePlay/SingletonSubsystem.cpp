// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SingletonSubsystem.h"

UWeaponFactory *USingletonSubsystem::WeaponFactory = nullptr;
URandomAwardBoxFactory *USingletonSubsystem::RandomAwardBoxFactory = nullptr;
UShop *USingletonSubsystem::Shop = nullptr;

USingletonSubsystem::~USingletonSubsystem()
{

}

void USingletonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USingletonSubsystem::Deinitialize()
{
	Super::Deinitialize();
	if(WeaponFactory)
	{
		WeaponFactory->RemoveFromRoot();
		WeaponFactory = nullptr;
	}
	if(RandomAwardBoxFactory)
	{
		RandomAwardBoxFactory->RemoveFromRoot();
		RandomAwardBoxFactory = nullptr;
	}
	if(Shop)
	{
		Shop->RemoveFromRoot();
		Shop = nullptr;
	}
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
		WeaponFactory->AddToRoot();
	}
	return WeaponFactory;
}

URandomAwardBoxFactory* USingletonSubsystem::GetRandomAwardBoxFactoryInst()
{
	if(RandomAwardBoxFactory==nullptr)
	{
		RandomAwardBoxFactory = NewObject<URandomAwardBoxFactory>();
		RandomAwardBoxFactory->AddToRoot();
	}
	return RandomAwardBoxFactory;
}

UShop* USingletonSubsystem::GetShopInst()
{
	if(Shop==nullptr)
	{
		Shop = NewObject<UShop>();
		Shop->AddToRoot();
	}
	return Shop;
}
