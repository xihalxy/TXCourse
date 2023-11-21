// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/WeaponBase.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE_API ABullet : public AWeaponBase
{
	GENERATED_BODY()

	ABullet();
public:
	void Fire();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	int BaseSpeed;

	int MaxLife;

	FTimerHandle LifeTimerHandle;
	
};


