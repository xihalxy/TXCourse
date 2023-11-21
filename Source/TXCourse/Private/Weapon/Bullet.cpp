// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"

ABullet::ABullet()
{
	BaseSpeed = 500;
	MaxLife = 10;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshInst(TEXT("/Script/Engine.StaticMesh'/Game/Weapon/PistolA_Ammo.PistolA_Ammo'"));
	if(StaticMeshInst.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshInst.Object);
	}
	//StaticMesh->SetRelativeRotation(FRotator(90,0,0));
	StaticMesh->SetUseCCD(true);
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveMentComponent"));
	ProjectileMovementComponent->InitialSpeed = 0;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

void ABullet::Fire()
{
	FVector Direction = GetActorRotation().Vector();
	Direction.Normalize();
	ProjectileMovementComponent->Velocity = Direction*BaseSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	StaticMesh->SetRelativeRotation(FRotator(-90,0,0)+GetActorRotation());
	GetWorldTimerManager().SetTimer(LifeTimerHandle,this,&ABullet::K2_DestroyActor,MaxLife);
	StartAttack();
}

void ABullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherComp->GetCollisionObjectType()==ECC_WorldStatic||OtherComp->GetCollisionObjectType()==ECC_WorldDynamic)
	{
		EndAttack();
		Destroy();
	}
}
