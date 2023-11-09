// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitTeamID);
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

	UPROPERTY(BlueprintAssignable)
	FInitTeamID InitTeamIDEvent;
	
private:
	UTouchInterface *TouchInterfaceInst;
	
};
