// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EShopItemType
{
	//武器
	MainWeapon = 0,
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

UCLASS(Blueprintable)
class TXCOURSE_API UShopItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,Replicated)
	int ItemID;
	
	UPROPERTY(BlueprintReadOnly,Replicated)
	EShopItemType ShopItemType;

	UPROPERTY(BlueprintReadOnly,Replicated)
	FName ItemName;

	UPROPERTY(BlueprintReadOnly,Replicated)
	int Price;

	UPROPERTY(BlueprintReadOnly,Replicated)
	int Weight;

	UPROPERTY(BlueprintReadOnly,Replicated)
	FString Desc;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* Widget;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
