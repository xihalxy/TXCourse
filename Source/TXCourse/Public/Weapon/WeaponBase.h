// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.generated.h"

class APlayerCharacter;
UCLASS()
class TXCOURSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	void InitWeapon(FString Name,int DamageValue,int HealthyValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDestroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual APlayerCharacter* GetPlayerCharacter();

	int GetDamage();

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Appearance)
	UStaticMeshComponent* StaticMesh;

	virtual APlayerCharacter* GetCharacter();

	UFUNCTION(Server,Reliable)
	void StartAttack();

	void EndAttack();

protected:

	FString WeaponName;

	int Damage;

	int Healthy;

};
