// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitTeamID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelay,float,DelayTime);
UCLASS()
class TXCOURSE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	AMyPlayerController();
	
public:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable)
	void SetView(FVector ScreenDistance,float Rate);

	void ReBirth();

	UFUNCTION(BlueprintCallable)
	void ResetPlayer();

	UPROPERTY(BlueprintAssignable)
	FInitTeamID InitTeamIDEvent;

	UPROPERTY(BlueprintAssignable)
	FDelay DelayForce;

	FTimerHandle RebirthTimer;
	
private:
	UTouchInterface *TouchInterfaceInst;
	
};
