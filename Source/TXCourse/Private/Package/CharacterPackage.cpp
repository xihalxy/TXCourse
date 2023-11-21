// Fill out your copyright notice in the Description page of Project Settings.


#include "Package/CharacterPackage.h"

#include "Character/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UPackageItem::UPackageItem()
{
	IsEmpty = true;
	IsUsed = false;
}

void UPackageItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPackageItem,IsEmpty);
	DOREPLIFETIME(UPackageItem,ID);
	DOREPLIFETIME(UPackageItem,ItemName);
	DOREPLIFETIME(UPackageItem,PackageItemType);
	DOREPLIFETIME(UPackageItem,Weight);
	DOREPLIFETIME(UPackageItem,IsUsed);
	DOREPLIFETIME(UPackageItem,Desc);
}

void UPackageItem::OnRep_IsEmpty()
{
	OnPackageItemChangeDelegate.Broadcast();
}

UCharacterPackage::UCharacterPackage()
{
	//InitPackage
	Capacity = 24;
	MoneyAmout = 0;
	TotalWeight = 0;
	Character = nullptr;
	for(int Cursor = 0;Cursor<Capacity;Cursor++)
	{
		Items.Emplace(NewObject<UPackageItem>());
		Items[Cursor]->ID = Cursor;
	}
}

UPackageItem* UCharacterPackage::RequestEmptyPackageItem()
{
	for(UPackageItem* Item : Items)
	{
		if(Item->IsEmpty)
		{
			Item->IsEmpty = false;
			Item->OnPackageItemChangeDelegate.Broadcast();
			return Item;
		}
	}
	return nullptr;
}

void UCharacterPackage::DeleteItem(int ItemID)
{
	if(ItemID<0||ItemID>=Capacity) return;
	Items[ItemID]->IsEmpty = true;
	Items[ItemID]->OnPackageItemChangeDelegate.Broadcast();
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
	TotalWeight = 0;
	for(UPackageItem* PackageItem:Items)
	{
		if(PackageItem->IsEmpty==false)
		{
			TotalWeight += PackageItem->Weight;
		}
	}
	return TotalWeight;
}

int UCharacterPackage::GetCapacity()
{
	return Capacity;
}

void UCharacterPackage::SwitchEquipment(int ID,bool Enable)
{
	if(ID>=Capacity||ID<0) return;
	if(Items[ID]->IsEmpty||Character==nullptr)
	{
		return;
	}
	if(Items[ID]->PackageItemType!=EPackageItemType::Pills&&Enable)
	{
		for(UPackageItem* PackageItem : Items)
		{
			if(PackageItem->IsUsed)
			{
				PackageItem->IsUsed = false;
				break;
			}
		}
	}
	Items[ID]->IsUsed=Enable;
	Character->SwitchEquipment(Items[ID],Enable);
}
