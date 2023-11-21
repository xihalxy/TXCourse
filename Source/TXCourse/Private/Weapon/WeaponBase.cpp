// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Ability/GE_WeaponHurt.h"
#include "Weapon/Ability/WeaponAttributeSet.h"

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
	BelongPlayerState = nullptr;
	BelongPlayerCharacter = nullptr;
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->InitCoolDown(1);
	AttributeSet->InitCost(10);
	AttributeSet->InitDamage(20);
	AbilitySystem->AddSpawnedAttribute(AttributeSet);
}

void AWeaponBase::InitWeapon(FString Name, int DamageValue, int HealthyValue,int CostValue,int CoolDownValue)
{
	WeaponName = Name;
	AbilitySystem->ApplyModToAttribute(UWeaponAttributeSet::GetDamageAttribute(),EGameplayModOp::Override,DamageValue);
	AbilitySystem->ApplyModToAttribute(UWeaponAttributeSet::GetCostAttribute(),EGameplayModOp::Override,CostValue);
	AbilitySystem->ApplyModToAttribute(UWeaponAttributeSet::GetCoolDownAttribute(),EGameplayModOp::Override,CoolDownValue);
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
	return BelongPlayerCharacter;
}

APlayerState* AWeaponBase::GetBelongPlayerState()
{
	return BelongPlayerState;
}

void AWeaponBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		if(Character == GetPlayerCharacter()) return;
		AbilitySystem->ApplyModToAttribute(UWeaponAttributeSet::GetAttackLuckyAttribute(),EGameplayModOp::Override,Character->AttributeSet->GetAttackLucky());
		Character->LastHitActorPlayerState = BelongPlayerState;
		FGameplayEventData* GEData = new FGameplayEventData();
		GEData->Instigator = this;
		GEData->Target = OtherActor;
		GEData->EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack.DealDamage"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetPlayerCharacter(),FGameplayTag::RequestGameplayTag(FName("Ability.Attack.DealDamage")),*GEData);
		AbilitySystem->ApplyGameplayEffectToTarget(NewObject<UGE_WeaponHurt>(),Character->AbilitySystem);
		EndAttack();
	}
}

void AWeaponBase::BindToPlayerCharacter(APlayerCharacter* PlayerCharacter)
{
	if(PlayerCharacter==nullptr) return;
	this->BelongPlayerCharacter = PlayerCharacter;
	BelongPlayerState = PlayerCharacter->GetPlayerState();
}

void AWeaponBase::StartAttack()
{
	SetActorEnableCollision(true);
}

void AWeaponBase::EndAttack()
{
	SetActorEnableCollision(false);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase,WeaponName);
	DOREPLIFETIME(AWeaponBase,BelongPlayerCharacter);
	DOREPLIFETIME(AWeaponBase,BelongPlayerState);
}



