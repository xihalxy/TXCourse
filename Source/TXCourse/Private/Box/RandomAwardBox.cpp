// Fill out your copyright notice in the Description page of Project Settings.


#include "Box/RandomAwardBox.h"

#include "GamePlay/SingletonSubsystem.h"
#include "Weapon/WeaponBase.h"

// Sets default values
ARandomAwardBox::ARandomAwardBox()
{
	SetReplicates(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr,TEXT("/Script/Engine.StaticMesh'/Game/AwardBox/AwardBox.AwardBox'"));
	if(StaticMesh!=nullptr)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->SetCollisionProfileName(FName("AwardBox"));
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetWorldScale3D(FVector(4));
	StaticMeshComponent->SetupAttachment(RootComponent);
	
}


void ARandomAwardBox::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		LastCollisionActor = Character;
		OnDestroyed();
	}
	else if(AWeaponBase* Weapon =Cast<AWeaponBase>(OtherActor))
	{
		LastCollisionActor = Weapon->GetPlayerCharacter();
		OnDestroyed();
	}
}

void ARandomAwardBox::InitBox(int awardType, FString objectName, int capacity,int weight,int order)
{
	this->AwardType = (EAwardType)awardType;
	this->ObjectName = objectName;
	this->Capacity = capacity;
	this->Weight = weight;
	this->Order = order;
}


// Called when the game starts or when spawned
void ARandomAwardBox::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this,&ARandomAwardBox::OnComponentBeginOverlap);
}

// Called every frame
void ARandomAwardBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARandomAwardBox::OnDestroyed()
{
	switch (AwardType)
	{
		case EAwardType::Weapon:
			{
				UPackageItem* PackageItem = LastCollisionActor->Package->RequestEmptyPackageItem();
				if(PackageItem == nullptr)
				{
					LastCollisionActor->Package->AddMoney(50);
				}
				PackageItem->IsUsed = false;
				PackageItem->ItemName = FName(ObjectName);
				PackageItem->Weight = Weight;
				if(Order == 0) PackageItem->PackageItemType = EPackageItemType::MainWeapon;
				else if(Order == 1)	PackageItem->PackageItemType = EPackageItemType::SecondWeapon;
				else if(Order == 2)	PackageItem->PackageItemType = EPackageItemType::ThirdWeapon;
				break;
			}
		default:
			break;
	}
	Destroy();
}

