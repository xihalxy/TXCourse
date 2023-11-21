// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Magic.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AMagic::AMagic()
{
	BaseSpeed = 400;
	DetectRange = 1000;
	UpdateTime = 0.2f;
	MaxLife = 100;
	AOERange = 500;
	MagicType = None;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshInst(TEXT("/Script/Engine.StaticMesh'/Game/Weapon/Magic/Sphere.Sphere'"));
	if(StaticMeshInst.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshInst.Object);
	}
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	TargetCharacter = nullptr;
	Fire_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire_Niagara"));
	Fire_NiagaraComponent->SetupAttachment(StaticMesh);
	Fire_NiagaraComponent->bAutoActivate = false;
	Fly_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fly_Niagara"));
	Fly_NiagaraComponent->SetupAttachment(StaticMesh);
	Fly_NiagaraComponent->bAutoActivate = false;
	Hit_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hit_Niagara"));
	Hit_NiagaraComponent->SetupAttachment(StaticMesh);
	Hit_NiagaraComponent->bAutoActivate = false;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveMentComponent"));
	ProjectileMovementComponent->InitialSpeed = 0;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

void AMagic::Fire()
{
	BeforeLunch();
	switch (MagicType) {
	case None:
		break;
	case Trace:
		StaticMesh->SetRelativeScale3D(FVector(0.3f));
		GetWorldTimerManager().SetTimer(TraceTimer,this,&AMagic::UpdateTrace,UpdateTime,true);
		break;
	case AOE:
		StaticMesh->SetRelativeScale3D(FVector(0.4f));
		break;
	case Line:
		StaticMesh->SetRelativeScale3D(FVector(0.5f));
		break;
	}
	FVector Direction = GetActorRotation().Vector();
	Direction.Normalize();
	ProjectileMovementComponent->Velocity = Direction*BaseSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ShowFireNiagrara();
	GetWorldTimerManager().SetTimer(LifeTimer,this,&AMagic::EndLife,MaxLife);
	StartAttack();
}

void AMagic::ShowFireNiagrara_Implementation()
{
	Fire_NiagaraComponent->Activate();
	Fly_NiagaraComponent->Activate();
}

void AMagic::ShowHitNiagrara_Implementation()
{
	Hit_NiagaraComponent->Activate();
	Fly_NiagaraComponent->Deactivate();
	StaticMesh->SetVisibility(false);
	GetWorldTimerManager().SetTimer(EndTimer,this,&AMagic::EndLife,1);
}

void AMagic::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		if(Character==GetPlayerCharacter()) return;
		if(EndTimer.IsValid()) return;
		ShowHitNiagrara();
	}else if(OtherComp->GetCollisionObjectType()==ECC_WorldStatic||OtherComp->GetCollisionObjectType()==ECC_WorldDynamic)
	{
		if(EndTimer.IsValid()) return;
		ShowHitNiagrara();
		EndAttack();
	}
}


void AMagic::BeforeLunch_Implementation()
{
	if(WeaponName.Contains("Trace"))
	{
		MagicType = EMagicType::Trace;
		UNiagaraSystem* NS_Fire = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkShot_02.NS_DarkShot_02'"));
		if(NS_Fire)
		{
			Fire_NiagaraComponent->SetAsset(NS_Fire);
		}
		UNiagaraSystem* NS_Fly = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkBullet_02.NS_DarkBullet_02'"));
		if(NS_Fly)
		{
			Fly_NiagaraComponent->SetAsset(NS_Fly);
		}
		UNiagaraSystem* NS_Hit = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkHit_02.NS_DarkHit_02'"));
		if(NS_Hit)
		{
			Hit_NiagaraComponent->SetAsset(NS_Hit);
		}
	}else if(WeaponName.Contains("AOE"))
	{
		MagicType = EMagicType::AOE;
		UNiagaraSystem* NS_Fire = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_BlueShot_01.NS_BlueShot_01'"));
		if(NS_Fire)
		{
			Fire_NiagaraComponent->SetAsset(NS_Fire);
		}
		UNiagaraSystem* NS_Fly = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_BlueBullet_01.NS_BlueBullet_01'"));
		if(NS_Fly)
		{
			Fly_NiagaraComponent->SetAsset(NS_Fly);
		}
		UNiagaraSystem* NS_Hit = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_BlueBullet_Hit_01.NS_BlueBullet_Hit_01'"));
		if(NS_Hit)
		{
			Hit_NiagaraComponent->SetAsset(NS_Hit);
		}
	}else if(WeaponName.Contains("Line"))
	{
		MagicType = EMagicType::Line;
		UNiagaraSystem* NS_Fire = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkFire_Shot_01.NS_DarkFire_Shot_01'"));
		if(NS_Fire)
		{
			Fire_NiagaraComponent->SetAsset(NS_Fire);
		}
		UNiagaraSystem* NS_Fly = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkFire_Bullet_01.NS_DarkFire_Bullet_01'"));
		if(NS_Fly)
		{
			Fly_NiagaraComponent->SetAsset(NS_Fly);
		}
		UNiagaraSystem* NS_Hit = LoadObject<UNiagaraSystem>(nullptr,TEXT("/Script/Niagara.NiagaraSystem'/Game/Weapon/Magic/NS_DarkFire_Hit_01.NS_DarkFire_Hit_01'"));
		if(NS_Hit)
		{
			Hit_NiagaraComponent->SetAsset(NS_Hit);
		}
	}
}

void AMagic::UpdateTrace_Implementation()
{
	FVector SelfLocation = GetActorLocation();
	float MinDistance = DetectRange;
	if(TargetCharacter)
	{
		if(TargetCharacter->bIsDeadth) MinDistance = DetectRange;
		else MinDistance = (TargetCharacter->GetActorLocation()-GetActorLocation()).Length();
	}
	for(APlayerCharacter* PlayerCharacter : TActorRange<APlayerCharacter>(GWorld))
	{
		if(PlayerCharacter==GetPlayerCharacter()) continue;
		float Distance = (PlayerCharacter->GetActorLocation()-SelfLocation).Length();
		if(Distance>MinDistance) continue;
		
		FCollisionQueryParams Params("Trace_Magic",false,this);
		Params.AddIgnoredActor(GetPlayerCharacter());
		Params.AddIgnoredActor(PlayerCharacter);
		TArray<AActor*> WeaponActor;
		UGameplayStatics::GetAllActorsOfClass(GWorld,AWeaponBase::StaticClass(),WeaponActor);
		Params.AddIgnoredActors(WeaponActor);
		bool bHit = GetWorld()->LineTraceTestByChannel(SelfLocation, PlayerCharacter->GetActorLocation(), ECC_Visibility, Params);
		if(bHit) continue;
		
		MinDistance = Distance;
		TargetCharacter = PlayerCharacter;
	}
	if(TargetCharacter)
	{
		FVector Direction = TargetCharacter->GetActorLocation()-GetActorLocation();
		Direction.Normalize();
		FVector OriginSpeed = ProjectileMovementComponent->Velocity;
		FVector TargetSpeed = Direction*BaseSpeed;
		FVector NewVelocity;
		NewVelocity.X = FMath::FInterpTo(OriginSpeed.X,TargetSpeed.X,UpdateTime,0.9);
		NewVelocity.Y = FMath::FInterpTo(OriginSpeed.Y,TargetSpeed.Y,UpdateTime,0.9);
		NewVelocity.Z = FMath::FInterpTo(OriginSpeed.Z,TargetSpeed.Z,UpdateTime,0.9);
		//StaticMesh->SetPhysicsLinearVelocity(NewVelocity);
		ProjectileMovementComponent->Velocity = NewVelocity;
	}
}

void AMagic::EndLife()
{
	Hit_NiagaraComponent->SetActive(false);
	EndAttack();
	if(MagicType==Trace)
	{
		GetWorldTimerManager().ClearTimer(TraceTimer);
	}
	GetWorldTimerManager().ClearTimer(LifeTimer);
	Destroy();
}
