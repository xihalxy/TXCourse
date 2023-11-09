// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API ABullet : public AWeaponBase
{
	GENERATED_BODY()


public:
	void InitBullet(FVector Direction,APlayerCharacter* LunchCharacter);

	virtual void BeginPlay() override;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual APlayerCharacter* GetCharacter() override;
private:

	FVector StartDirection;

	float LunchForceRate;

	APlayerCharacter* Character;
};


