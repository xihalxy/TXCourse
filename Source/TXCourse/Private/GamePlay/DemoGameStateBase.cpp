// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DemoGameStateBase.h"

#include "GamePlay/DemoGameInstance.h"
#include "GamePlay/DemoPlayerState.h"


ADemoGameStateBase::ADemoGameStateBase()
{
	ScoreMap.Init(0,10);
}

void ADemoGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADemoGameStateBase::NotifyTeamScoreChange(int TeamID)
{
	for(APlayerState* PlayerState : PlayerArray)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			if(DemoPlayerState->TeamID==TeamID)
			{
				DemoPlayerState->TeamScore = ScoreMap[TeamID];
				DemoPlayerState->OnScoreChange();
			}
		}
	}
}


void ADemoGameStateBase::StartGame()
{
	StartTime = GetServerWorldTimeSeconds();
	GetWorldTimerManager().SetTimer(TimeCountHandle,this,&ADemoGameStateBase::UpdateGameTime,1,true);
	GetGameInstance<UDemoGameInstance>()->GameStage = EGameStage::Loop;
	GetWorldTimerManager().ClearTimer(PreLoopCheckHandle);
}

void ADemoGameStateBase::EndGame()
{
	GetWorldTimerManager().ClearTimer(TimeCountHandle);
	GetGameInstance<UDemoGameInstance>()->GameStage = EGameStage::End;
}

float ADemoGameStateBase::GetGameTime()
{
	return LastTime;
}



void ADemoGameStateBase::UpdateGameTime()
{
	LastTime = GetServerWorldTimeSeconds()-StartTime;
}
