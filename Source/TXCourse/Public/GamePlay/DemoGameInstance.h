// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DemoGameInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameStage : uint8
{
	PreGame=0,
	Loop,
	End
};

UCLASS()
class TXCOURSE_API UDemoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable,Category="Net")
	void ConnectToServer(FString IpAddress);

	UPROPERTY(BlueprintReadOnly)
	EGameStage GameStage = EGameStage::PreGame;
	
};
