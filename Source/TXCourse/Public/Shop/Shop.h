// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shop.generated.h"

class APlayerCharacter;
class UShopItem;
/**
 * 
 */
UCLASS()
class TXCOURSE_API UShop : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	TArray<UShopItem*> GetItemList();

	UFUNCTION(BlueprintCallable)
	void GetItem(int ID,APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void EnterShop(APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void LeaveShop(APlayerCharacter* PlayerCharacter);
	
private:
	UShop();

	UFUNCTION(Server,Reliable)
	void ReadXML();

	UPROPERTY(Replicated)
	TArray<UShopItem*> ItemList;

	TMap<APlayerCharacter*,FVector> PlayerInShop;

	FVector ShopLocation;

	FVector LeaveLocation;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
