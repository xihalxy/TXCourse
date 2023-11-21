// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/Shop.h"
#include "XmlFile.h"
#include "Character/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Shop/ShopItem.h"

TArray<UShopItem*> UShop::GetItemList()
{
	return ItemList;
}

void UShop::GetItem(int ID, APlayerCharacter* PlayerCharacter)
{
	if(ID<0||ID>=ItemList.Num()) return;
	if(PlayerCharacter&&PlayerCharacter->Package)
	{
		if(PlayerCharacter->Package->GetMoneyAmount()>=ItemList[ID]->Price)
		{
			UPackageItem* PackageItem = PlayerCharacter->Package->RequestEmptyPackageItem();
			if(PackageItem==nullptr) return;
			
			PlayerCharacter->Package->AddMoney(-ItemList[ID]->Price);
			PackageItem->Desc = ItemList[ID]->Desc;
			PackageItem->ItemName = ItemList[ID]->ItemName;
			PackageItem->Weight = ItemList[ID]->Weight;
			PackageItem->PackageItemType = (EPackageItemType)ItemList[ID]->ShopItemType;
		}
		
	}
}

void UShop::EnterShop_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerInShop.Add(PlayerCharacter,PlayerCharacter->GetActorLocation());
	PlayerCharacter->SetActorLocation(ShopLocation);
}

void UShop::LeaveShop_Implementation(APlayerCharacter* PlayerCharacter)
{
	if(PlayerInShop.Contains(PlayerCharacter))
	{
		PlayerCharacter->SetActorLocation(PlayerInShop[PlayerCharacter]+FVector(200,0,0));
		PlayerInShop.Remove(PlayerCharacter);
	}else
	{
		PlayerCharacter->SetActorLocation(LeaveLocation);
	}
}

UShop::UShop()
{
	ShopLocation = FVector(0);
	LeaveLocation = FVector(-3130.000000,-380.000000,-80.000000);
	ReadXML();
}

void UShop::ReadXML_Implementation()
{
	FString filePath = FPaths::Combine(FPaths::ProjectContentDir(),FString("XMLFiles"),FString("ShopItem.xml"));
	FXmlFile*xml = new FXmlFile();
	xml->LoadFile(filePath);
	check(xml->IsValid());
	FXmlNode* RootNode = xml->GetRootNode();
	TArray<FXmlNode*> XmlNodes = RootNode->GetChildrenNodes();
	int Count = 0;
	for(FXmlNode* Node : XmlNodes)
	{
		UShopItem* ShopItem = NewObject<UShopItem>();
		if(ShopItem)
		{
			ShopItem->ItemID = Count;
			ShopItem->ItemName = FName(Node->FindChildNode("ItemName")->GetContent());
			ShopItem->ShopItemType = (EShopItemType)FCString::Atoi(*(Node->FindChildNode("ItemType")->GetContent()));
			ShopItem->Price =  FCString::Atoi(*(Node->FindChildNode("Price")->GetContent()));
			ShopItem->Desc = Node->FindChildNode("Desc")->GetContent();
			ShopItem->Weight = FCString::Atoi(*(Node->FindChildNode("Weight")->GetContent()));
			ItemList.Add(ShopItem);
			Count++;
		}
	}
}

void UShop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShop,ItemList);
}

