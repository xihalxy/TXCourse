// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPackage.generated.h"

/**
 * 
 */

class APlayerCharacter;
UENUM(BlueprintType)
enum class EPackageItemType : uint8
{
	//武器
	MainWeapon,
	SecondWeapon,
	ThirdWeapon,

	//防具
	HeadEquip,
	UpperEquip,
	DownEquip,
	
	//技能
	MainSkillBook,
	SecondSkillBook,
	
	//药水
	Pills
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPackageItemChange);
UCLASS(Blueprintable)
class TXCOURSE_API UPackageItem : public UObject
{
	GENERATED_BODY()

	UPackageItem();
public:

	UPROPERTY(BlueprintReadOnly,Category="PackageItem",ReplicatedUsing=OnRep_IsEmpty)
	bool IsEmpty;
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	int ID;
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	FName ItemName;
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	EPackageItemType PackageItemType;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	int Weight;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	bool IsUsed;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem",Replicated)
	FString Desc;

	UPROPERTY(BlueprintAssignable,Category="PackageItem",Replicated)
	FOnPackageItemChange OnPackageItemChangeDelegate;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_IsEmpty();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoneyChange);

UCLASS(Blueprintable)
class TXCOURSE_API UCharacterPackage : public UObject
{
	GENERATED_BODY()

	UCharacterPackage();


public:

	UPackageItem* RequestEmptyPackageItem(); 

	UFUNCTION(BlueprintCallable,Category="PackageItem")
	void DeleteItem(int ItemID);
	
	UFUNCTION(BlueprintCallable,Category="PackageItem")
	void MoneySpend(int Value);
	
	UFUNCTION(BlueprintCallable,Category="PackageItem")
	int GetMoneyAmount();

	UFUNCTION(BlueprintCallable,Category="PackageItem")
	void AddMoney(int NewMoney);

	UFUNCTION(BlueprintCallable,Category="PackageItem")
	int GetTotalWeight();

	UFUNCTION(BlueprintCallable,Category="PackageItem")
	int GetCapacity();
	
	UPROPERTY(BlueprintAssignable,Category="PackageItem")
	FOnMoneyChange MoneyChangeEvent;
	
	UFUNCTION(BlueprintCallable,Category="PackageItem")
	void SwitchEquipment(int ID,bool Enable);

	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	TArray<UPackageItem*> Items;
	
	APlayerCharacter* Character;

private:
	
	int Capacity;
	
	int MoneyAmout;
	
	int TotalWeight;

};
