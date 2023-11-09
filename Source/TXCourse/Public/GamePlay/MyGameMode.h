// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class ADemoPlayerState;
/**
 * 
 */
UENUM()
enum EGameMode
{
	Time,
	Place
};

UCLASS()
class TXCOURSE_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	AMyGameMode();
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual  void BeginPlay() override;

	void AddScore(ADemoPlayerState* TargetPlayerState);

	EGameMode ModeType;

	float GameMaxTime;

private:

	int BaseScore;
	
};
