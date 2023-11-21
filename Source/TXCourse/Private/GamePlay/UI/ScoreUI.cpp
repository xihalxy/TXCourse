// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/UI/ScoreUI.h"

#include "GamePlay/DemoGameStateBase.h"
#include "GamePlay/DemoPlayerState.h"


void UScoreUI::UpdateSelfScore(int NewScore)
{
	SelfScore = NewScore;
	SelfScoreChangeEvent.Broadcast();
}

void UScoreUI::UpdateTeamScore(int NewScore)
{
	TeamScore = NewScore;
	TeamScoreChangeEvent.Broadcast();
}

void UScoreUI::UpdateScore(int NewSelfScore, int NewTeamScore)
{
	SelfScore = NewSelfScore;
	TeamScore = NewTeamScore;
	ScoreChangeEvent.Broadcast();
}

void UScoreUI::InitSlate()
{
	SelfScore = 0;
	TeamScore = 0;
	GWorld->GetTimerManager().SetTimer(TimeHandle,this,&UScoreUI::UpdateTime,1,true);
}

void UScoreUI::UpdateTime()
{
	if(ADemoGameStateBase *GameState = GWorld->GetGameState<ADemoGameStateBase>())
	{
		TimeChangeEvent.Broadcast(GameState->GetGameTime());
	}
}
