// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Magic.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;
/**
 * 
 */
class UNiagaraComponent;
class UPawnSensingComponent;
class APlayerCharacter;

UENUM()
enum EMagicType
{
	None,
	Trace,
	AOE,
	Line
};
UCLASS()
class TXCOURSE_API AMagic : public AWeaponBase
{
	GENERATED_BODY()

	AMagic();

public:

	void Fire();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UNiagaraComponent* Fire_NiagaraComponent;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UNiagaraComponent* Fly_NiagaraComponent;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UNiagaraComponent* Hit_NiagaraComponent;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(NetMulticast,Unreliable)
	void ShowFireNiagrara();

	UFUNCTION(NetMulticast,Unreliable)
	void ShowHitNiagrara();


	
	
private:
	
	APlayerCharacter* TargetCharacter;
	
	int BaseSpeed;
	
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(NetMulticast,Reliable)
	void BeforeLunch();

	UFUNCTION(Server,Reliable)
	void UpdateTrace();

	void EndLife();

	int AOERange;

	int DetectRange;

	int MaxLife;

	float UpdateTime;
	
	EMagicType MagicType;

	FTimerHandle LifeTimer;

	FTimerHandle EndTimer;

	FTimerHandle TraceTimer;
	
};
