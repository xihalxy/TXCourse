// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponFactory.h"

#include "XmlFile.h"
#include "Weapon/Bomb.h"
#include "Weapon/Bullet.h"
#include "Weapon/Gun.h"
#include "Weapon/Sword.h"
#include "Weapon/WeaponBase.h"


UWeaponFactory::UWeaponFactory()
{
	ReadXml();
}

AWeaponBase* UWeaponFactory::SpawnWeapon(FString WeaponName, FName SocketName, USkeletalMeshComponent* Mesh)
{
	if(GWorld)
	{
		if(Cast<USkeletalMeshComponent>(Mesh)==nullptr)
		{
			UE_LOG(LogActor,Warning,TEXT("Binding Mesh is not Valid"));
			return nullptr;
		}
		if(WeaponInfos.Find(WeaponName)==nullptr)
		{
			UE_LOG(LogActor,Warning,TEXT("%s does not exist in weapon list"));
			return nullptr;
		}
		AWeaponBase* Weapon;
		if(WeaponName.Contains("Sword"))
		{
			Weapon = GWorld->SpawnActor<ASword>();
		}else if(WeaponName.Contains("Bullet"))
		{
			Weapon = GWorld->SpawnActor<ABullet>();
		}else if(WeaponName.Contains("Gun"))
		{
			Weapon = GWorld->SpawnActor<AGun>();
		}else if(WeaponName.Contains("Bomb"))
		{
			Weapon = GWorld->SpawnActor<ABomb>();
		}else
		{
			return nullptr;
		}
		FWeaponStruct* WeaponInfo = WeaponInfos[WeaponName];
		Weapon->InitWeapon(WeaponInfo->WeaponName,WeaponInfo->Damage,WeaponInfo->Healthy);
		Weapon->AttachToComponent(Mesh,FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
		return  Weapon;
	}
	return nullptr;
}

void UWeaponFactory::ReadXml()
{
	FString filePath = FPaths::Combine(FPaths::ProjectContentDir(),FString("Weapon"),FString("Weapon.xml"));
	FXmlFile*xml = new FXmlFile();
	xml->LoadFile(filePath);
	check(xml->IsValid());
	FXmlNode* RootNode = xml->GetRootNode();
	TArray<FXmlNode*> XmlNodes = RootNode->GetChildrenNodes();
	for(FXmlNode* Node : XmlNodes)
	{
		FWeaponStruct* WeaponInfo = new FWeaponStruct();
		WeaponInfo->Damage = FCString::Atoi(*(Node->FindChildNode("Damage")->GetContent()));
		WeaponInfo->Healthy = FCString::Atoi(*(Node->FindChildNode("Healthy")->GetContent()));
		WeaponInfo->WeaponName = Node->FindChildNode("Name")->GetContent();
		WeaponInfos.Add(WeaponInfo->WeaponName,WeaponInfo);
	}
	
}
