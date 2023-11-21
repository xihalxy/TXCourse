// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponFactory.h"

#include "XmlFile.h"
#include "Weapon/Bomb.h"
#include "Weapon/Bullet.h"
#include "Weapon/Gun.h"
#include "Weapon/Magic.h"
#include "Weapon/MagicStick.h"
#include "Weapon/Sword.h"
#include "Weapon/WeaponBase.h"


UWeaponFactory::UWeaponFactory()
{
	ReadXml();
}
//use for projectile
AWeaponBase* UWeaponFactory::SPawnWeapon(FString WeaponName,FVector Location,FRotator Rotator)
{
	if(GWorld)
	{
		if(WeaponInfos.Find(WeaponName)==nullptr)
		{
			UE_LOG(LogActor,Warning,TEXT("%s does not exist in weapon list"),*WeaponName);
			return nullptr;
		}
		FActorSpawnParameters param = FActorSpawnParameters();
		param.bNoFail = true;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeaponBase* Weapon;
		if(WeaponName.Contains("Bullet"))
		{
			Weapon = GWorld->SpawnActor<ABullet>(ABullet::StaticClass(),Location,Rotator);
		}else if(WeaponName.Contains("_Magic"))
		{
			Weapon = GWorld->SpawnActor<AMagic>(AMagic::StaticClass(),Location,Rotator);
		}else
		{
			return nullptr;
		}
		FWeaponStruct* WeaponInfo = WeaponInfos[WeaponName];
		Weapon->InitWeapon(WeaponInfo->WeaponName,WeaponInfo->Damage,WeaponInfo->Healthy,0,0);
		return Weapon;
	}
	return nullptr;
}
//use for hand weapon
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
			UE_LOG(LogActor,Warning,TEXT("%s does not exist in weapon list"),*WeaponName);
			return nullptr;
		}
		AWeaponBase* Weapon;
		if(WeaponName.Contains("Sword"))
		{
			Weapon = GWorld->SpawnActor<ASword>();
		}else if(WeaponName.Contains("Gun"))
		{
			Weapon = GWorld->SpawnActor<AGun>();
		}else if(WeaponName.Contains("Bomb"))
		{
			Weapon = GWorld->SpawnActor<ABomb>();
		}else if(WeaponName.Contains("Stick"))
		{
			Weapon = GWorld->SpawnActor<AMagicStick>();
		}else
		{
			return nullptr;
		}
		FWeaponStruct* WeaponInfo = WeaponInfos[WeaponName];
		Weapon->InitWeapon(WeaponInfo->WeaponName,WeaponInfo->Damage,WeaponInfo->Healthy,WeaponInfo->Cost,WeaponInfo->Cooldown);
		Weapon->AttachToComponent(Mesh,FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
		return  Weapon;
	}
	return nullptr;
}

void UWeaponFactory::ReadXml()
{
	FString filePath = FPaths::Combine(FPaths::ProjectContentDir(),FString("XMLFiles"),FString("Weapon.xml"));
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
		WeaponInfo->Cost = FCString::Atoi(*(Node->FindChildNode("Cost")->GetContent()));
		WeaponInfo->Cooldown = FCString::Atoi(*(Node->FindChildNode("Cooldown")->GetContent()));
		WeaponInfo->WeaponName = Node->FindChildNode("Name")->GetContent();
		WeaponInfos.Add(WeaponInfo->WeaponName,WeaponInfo);
	}
	
}
