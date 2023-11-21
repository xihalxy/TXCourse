// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"

ASword::ASword()
{
	UStaticMesh* StaticMeshResouce = LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Weapon/SM_Sword.SM_Sword"));
	check(StaticMeshResouce);
	StaticMesh->SetStaticMesh(StaticMeshResouce);
}
