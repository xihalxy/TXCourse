// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "UObject/NoExportTypes.h"
#include "WeaponFactory.generated.h"

/**
 * 
 */
USTRUCT()
struct FWeaponStruct
{
	GENERATED_BODY()

	FString WeaponName;

	int Damage;

	int Healthy;

	int Cost;

	int Cooldown;
};
UCLASS()
class TXCOURSE_API UWeaponFactory : public UObject
{
	GENERATED_BODY()


public:
	
	AWeaponBase* SPawnWeapon(FString WeaponName,FVector Location,FRotator Rotator);
	
	AWeaponBase* SpawnWeapon(FString WeaponName, FName SocketName, USkeletalMeshComponent* Mesh);

private:
	
	UWeaponFactory();

	void ReadXml();

	TMap<FString,FWeaponStruct*> WeaponInfos;

	
};
