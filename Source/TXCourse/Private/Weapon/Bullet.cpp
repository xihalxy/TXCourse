// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"

void ABullet::InitBullet(FVector Direction,APlayerCharacter* LunchCharacter)
{
	/*UStaticMesh* StaticMeshResouce = LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Weapon/SM_Bullet.SM_Bullet"));
	check(StaticMeshResouce);
	StaticMesh->SetStaticMesh(StaticMeshResouce);*/
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetMassOverrideInKg(NAME_None,0.05);
	LunchForceRate = 1000.f;
	StartDirection = Direction;
	Character = LunchCharacter;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->AddForce(StartDirection*LunchForceRate);
}

void ABullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(GetVelocity().Length()<100)
	{
		Destroy();
	}else
	{
		FVector Direction = GetVelocity();
		Direction.Normalize();
		float ForceRate = 3.f;
		StaticMesh->AddForce(-Direction*ForceRate);
	}
	
}
APlayerCharacter* ABullet::GetCharacter()
{
	return Character;
}
