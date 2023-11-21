// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MagicStick.h"
#include "GamePlay/SingletonSubsystem.h"
#include "Weapon/Magic.h"

AMagicStick::AMagicStick()
{
	UStaticMesh* StaticMeshResouce = LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Weapon/SM_Mace.SM_Mace"));
	check(StaticMeshResouce);
	StaticMesh->SetStaticMesh(StaticMeshResouce);
}

void AMagicStick::StartAttack()
{
	Super::StartAttack();
	AMagic* Magic =Cast<AMagic>(GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SPawnWeapon(WeaponName+"_Magic",GetPlayerCharacter()->weapon_r->GetActorLocation(),GetPlayerCharacter()->GetAimRotator()));
	Magic->BindToPlayerCharacter(BelongPlayerCharacter);
	Magic->Fire();
}

void AMagicStick::EndAttack()
{
	Super::EndAttack();
	
}
