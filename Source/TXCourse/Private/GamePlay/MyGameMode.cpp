// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/MyGameMode.h"

#include "GamePlay/DemoGameStateBase.h"
#include "GamePlay/DemoPlayerState.h"


AMyGameMode::AMyGameMode()
{
	//PlayerControllerClass = AMyPlayerController::StaticClass();
	//GameStateClass = ADemoGameStateBase::StaticClass();
	BaseScore = 1;
	bUseSeamlessTravel = true;
	GameMaxTime = 300;
}

AActor* AMyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const FString PlayerStartTag = FString("Start_R");
	return FindPlayerStart(Player,PlayerStartTag);
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGameMode::AddScore(ADemoPlayerState* TargetPlayerState)
{
	TargetPlayerState->SelfScore+=BaseScore;
	if(ADemoGameStateBase* DGameState = GetGameState<ADemoGameStateBase>())
	{
		int ScoreTeamID = TargetPlayerState->TeamID;
		if(ScoreTeamID>=0 && ScoreTeamID<10)
		{
			DGameState->ScoreMap[ScoreTeamID]+=BaseScore;
			DGameState->NotifyTeamScoreChange(ScoreTeamID);
		}
	}
}


