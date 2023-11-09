// Fill out your copyright notice in the Description page of Project Settings.


#include "Package/CharacterPackage.h"

#include "Character/PlayerCharacter.h"

UCharacterPackage::UCharacterPackage()
{
	//InitPackage
	Capacity = 24;
	MoneyAmout = 0;
	TotalWeight = 0;
	Character = nullptr;
	for(int i = 0 ;i<Capacity;i++)
	{
		IsEmptyTable.Add(i,true);
	}
}


bool UCharacterPackage::AddItem(TSharedPtr<FPackageItemStruct> Item)
{
	for(int Cursor = 0;Cursor<Capacity;Cursor++)
	{
		if(IsEmptyTable.Find(Cursor))
		{
			IsEmptyTable[Cursor] = false;
			Item->ID = Cursor;
			ItemList.Add(Cursor,Item);
			TotalWeight+=Item->Weight;
			PackageChangeEvent.Broadcast(*Item);
			return true;
		}
	}
	return false;
}

void UCharacterPackage::DeleteItem(int ItemID)
{
	if(ItemID>=Capacity) return;
	TSharedPtr<FPackageItemStruct> Item = ItemList[ItemID];
	if(Item)
	{
		if(Item->IsUsed)
		{
			SwitchEquipment(ItemID,false);
		}
		TotalWeight-=Item->Weight;
		ItemList.Remove(ItemID);	
		IsEmptyTable[ItemID] = true;
	}
}

void UCharacterPackage::MoneySpend(int Value)
{
	MoneyAmout-=Value;
	MoneyChangeEvent.Broadcast();
}

int UCharacterPackage::GetMoneyAmount()
{
	return MoneyAmout;
}

void UCharacterPackage::AddMoney(int NewMoney)
{
	MoneyAmout += NewMoney;
	MoneyChangeEvent.Broadcast();
}

int UCharacterPackage::GetTotalWeight()
{
	return TotalWeight;
}

int UCharacterPackage::GetCapacity()
{
	return Capacity;
}

void UCharacterPackage::SwitchEquipment(int ID,bool Enable)
{
	if(ID>=Capacity) return;
	if(IsEmptyTable[ID]||Character==nullptr)
	{
		return;
	}
	ItemList[ID]->IsUsed=Enable;
	Character->SwitchEquipment(*ItemList[ID],Enable);
}
