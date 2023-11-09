// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Box/RandomAwardBox.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(FName("Weapon"));
	StaticMesh->SetIsReplicated(true);
}

void AWeaponBase::InitWeapon(FString Name, int DamageValue, int HealthyValue)
{
	WeaponName = Name;
	Damage = DamageValue;
	Healthy = HealthyValue;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this,&AWeaponBase::OnComponentBeginOverlap);
	SetActorEnableCollision(false);
}

void AWeaponBase::OnDestroyed()
{
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APlayerCharacter* AWeaponBase::GetPlayerCharacter()
{
	return Cast<APlayerCharacter>(GetAttachParentActor());
}

int AWeaponBase::GetDamage()
{
	return Damage;
}

void AWeaponBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		if(Character == GetPlayerCharacter()) return;
		Character->LastHitActor = GetPlayerCharacter();
		FGameplayEventData* GEData = new FGameplayEventData();
		GEData->Instigator = this;
		GEData->Target = OtherActor;
		GEData->EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack.DealDamage"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetPlayerCharacter(),FGameplayTag::RequestGameplayTag(FName("Ability.Attack.DealDamage")),*GEData);
	}
}

APlayerCharacter* AWeaponBase::GetCharacter()
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetAttachParentActor());
	return Character;
}

void AWeaponBase::StartAttack_Implementation()
{
	SetActorEnableCollision(true);
}



void AWeaponBase::EndAttack()
{
	SetActorEnableCollision(false);
}



