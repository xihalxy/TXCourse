// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NoneWeapon.h"


ANoneWeapon::ANoneWeapon()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("CollisionCheck");
	SphereComponent->SetSphereRadius(70.f);
	SphereComponent->SetVisibility(false);
	SphereComponent->SetupAttachment(StaticMesh);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ANoneWeapon::OnComponentBeginOverlap);
}


