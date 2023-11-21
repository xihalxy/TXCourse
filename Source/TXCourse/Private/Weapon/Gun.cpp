// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Gun.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GamePlay/SingletonSubsystem.h"
#include "Weapon/Bullet.h"

AGun::AGun()
{
	UStaticMesh* StaticMeshResouce = LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Weapon/Pistol.Pistol"));
	check(StaticMeshResouce);
	StaticMesh->SetStaticMesh(StaticMeshResouce);
	Fire_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire_Niagara"));
	Fire_NiagaraComponent->SetupAttachment(StaticMesh);
	Fire_NiagaraComponent->SetRelativeRotation(FRotator(0,270,0));
	Fire_NiagaraComponent->bAutoActivate = false;
	UNiagaraSystem* NS_Fire = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Action_FX_02/NiagaraSystems/FireBullet/NS_Fire_Shot_01.NS_Fire_Shot_01'"));
	if(NS_Fire&&Fire_NiagaraComponent)
	{
		Fire_NiagaraComponent->SetAsset(NS_Fire);
	}
}

void AGun::StartAttack()
{
	Super::StartAttack();
	ABullet* Bullet =Cast<ABullet>(GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SPawnWeapon(WeaponName+"_Bullet",GetPlayerCharacter()->weapon_r->GetActorLocation(),GetPlayerCharacter()->GetAimRotator()));
	Bullet->BindToPlayerCharacter(BelongPlayerCharacter);
	Bullet->Fire();
	ShowFireNiagara();
}

void AGun::EndAttack()
{
	Super::EndAttack();
}

void AGun::StopFireNiagara_Implementation()
{
	Fire_NiagaraComponent->Deactivate();
}

void AGun::ShowFireNiagara_Implementation()
{
	Fire_NiagaraComponent->Activate(true);
	GetWorldTimerManager().SetTimer(FireNiagaraTimer,this,&AGun::StopFireNiagara,1);
}
