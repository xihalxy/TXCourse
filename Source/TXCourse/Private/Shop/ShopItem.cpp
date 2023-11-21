// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopItem.h"

#include "Net/UnrealNetwork.h"


void UShopItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShopItem,ItemID)
	DOREPLIFETIME(UShopItem,ItemName);
	DOREPLIFETIME(UShopItem,Price);
	DOREPLIFETIME(UShopItem,ShopItemType);
	DOREPLIFETIME(UShopItem,Weight);
	DOREPLIFETIME(UShopItem,Desc);
}
