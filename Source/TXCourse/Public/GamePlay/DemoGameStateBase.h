// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemoGameStateBase.generated.h"
class ADemoPlayerState;
/**
 * 
 */
UCLASS()
class TXCOURSE_API ADemoGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ADemoGameStateBase();
public:
	
	virtual void BeginPlay() override;
	
	TArray<int> ScoreMap;

	void NotifyTeamScoreChange(int TeamID);
	
	void StartGame();

	void EndGame();

	float GetGameTime();

private:

	FTimerHandle TimeCountHandle;

	FTimerHandle CountDownHandle;

	FTimerHandle PreLoopCheckHandle;
	
	float StartTime;

	float LastTime;
	
	void UpdateGameTime();
	
	

	
};
