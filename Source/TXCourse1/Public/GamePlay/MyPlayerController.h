// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TXCOURSE1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	AMyPlayerController();
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetView(FVector ScreenDistance,float Rate);
	
private:
	UTouchInterface *TouchInterfaceInst;
	
};
