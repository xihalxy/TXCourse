// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
USTRUCT(BlueprintType)
struct FPackageItemStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	int ID;
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	FName ItemName;
	
	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	EPackageItemType PackageItemType;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	int Weight;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	bool IsUsed;

	UPROPERTY(BlueprintReadOnly,Category="PackageItem")
	FString Desc;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPackageChange,FPackageItemStruct,PackageItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoneyChange);
UCLASS(Blueprintable)
class TXCOURSE_API UCharacterPackage : public UObject
{
	GENERATED_BODY()

	UCharacterPackage();


public:

	//UFUNCTION(BlueprintCallable,Category="PackageItem")
	bool AddItem(TSharedPtr<FPackageItemStruct> Item);

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
	FOnPackageChange PackageChangeEvent;
	UPROPERTY(BlueprintAssignable,Category="PackageItem")
	FOnMoneyChange MoneyChangeEvent;

	UFUNCTION(BlueprintCallable,Category="PackageItem")
	void SwitchEquipment(int ID,bool Enable);

	APlayerCharacter* Character;

private:
	
	int Capacity;
	
	int MoneyAmout;
	
	int TotalWeight;
	
	TMap<int,TSharedPtr<FPackageItemStruct>> ItemList;
	TMap<int,bool> IsEmptyTable;
};
