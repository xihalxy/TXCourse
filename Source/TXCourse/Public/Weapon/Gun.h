// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Gun.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class TXCOURSE_API AGun : public AWeaponBase
{
	GENERATED_BODY()

	AGun();

public:
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UNiagaraComponent* Fire_NiagaraComponent;

	UFUNCTION(BlueprintCallable)
	virtual void StartAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void EndAttack() override;

	UFUNCTION(NetMulticast,Reliable)
	void ShowFireNiagara();

	UFUNCTION(NetMulticast,Reliable)
	void StopFireNiagara();

	FTimerHandle FireNiagaraTimer;
};
