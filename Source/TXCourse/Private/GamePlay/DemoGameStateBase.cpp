// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DemoGameStateBase.h"

#include "Character/PlayerCharacter.h"
#include "GamePlay/DemoGameInstance.h"
#include "GamePlay/DemoPlayerState.h"
#include "GamePlay/MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ADemoGameStateBase::ADemoGameStateBase()
{
	MaxTeam = 10;
	ScoreMap.Init(-1,MaxTeam);
}

void ADemoGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole()==ROLE_Authority)
	{
		StartGame();
	}
}

void ADemoGameStateBase::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
	{
		if(DemoPlayerState->TeamID>=0&&DemoPlayerState->TeamID<MaxTeam)
		{
			ScoreMap[DemoPlayerState->TeamID] = 0;
		}
	}
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
	StartTime = FMath::RoundToInt(GetServerWorldTimeSeconds());
	GetWorldTimerManager().SetTimer(TimeCountHandle,this,&ADemoGameStateBase::UpdateGameTime,1,true);
	GetWorldTimerManager().SetTimer(TotalGameTimer,this,&ADemoGameStateBase::EndGame,Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this))->GameMaxTime);
	GetGameInstance<UDemoGameInstance>()->GameStage = EGameStage::Loop;
}

void ADemoGameStateBase::EndGame()
{
	if(CheckNeedExtraTime())
	{
		GetWorldTimerManager().SetTimer(TotalGameTimer,this,&ADemoGameStateBase::EndGame,60);
	}else
	{
		GetWorldTimerManager().ClearTimer(TimeCountHandle);
		GetGameInstance<UDemoGameInstance>()->GameStage = EGameStage::End;
		CalcPlayerRank();
	}
}

void ADemoGameStateBase::OnRep_LastTime()
{
	TimeChangeDelegate.Broadcast(LastTime);
}

float ADemoGameStateBase::GetGameTime()
{
	return LastTime;
}

void ADemoGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoGameStateBase,LastTime);
}

void ADemoGameStateBase::PlayerReset_Implementation(ACharacter* Character)
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
	{
		if(PlayerCharacter->GetController())
		{
			PlayerCharacter->SetActorLocation(UGameplayStatics::GetGameMode(GWorld)->FindPlayerStart(PlayerCharacter->GetController())->GetActorLocation());
		}
	}
}

void ADemoGameStateBase::PlayerReBirth_Implementation(APlayerController* PlayerController)
{
	if(PlayerController) UGameplayStatics::GetGameMode(GWorld)->RestartPlayer(PlayerController);	
}


void ADemoGameStateBase::UpdateGameTime()
{
	LastTime = FMath::RoundToInt(GetServerWorldTimeSeconds())-StartTime;
	TimeChangeDelegate.Broadcast(LastTime);
}

void ADemoGameStateBase::CalcPlayerRank()
{
	
	//calc Victory Team
	int VictoryTeam = 0;//mem Victory Team ID
	int CurrentMax = -1;
	for(int i = 0;i<ScoreMap.Num();i++)
	{
		if(ScoreMap[i]>CurrentMax)
		{
			VictoryTeam = i;
			CurrentMax = ScoreMap[i];
		}
	}
	
	//calc TeamRank
	int TotalTeamCount = 0; //mem total Team number
	TMap<int,int> TeamSortIndex; //mem Team Rank by Score
	TArray<int> SortArray;
	for(int i=0;i<ScoreMap.Num();i++)
	{
		if(ScoreMap[i]<0) continue;
		TotalTeamCount ++;
		TeamSortIndex.FindOrAdd(ScoreMap[i],-1);
		SortArray.Add(ScoreMap[i]);
	}
	SortArray.Sort();
	int RankIndex = TotalTeamCount;
	for(int Score : SortArray)
	{
		TeamSortIndex[Score]=RankIndex;
		RankIndex--;
	}
	
	SortArray.Empty();
	UE_LOG(LogGameState,Log,TEXT("TeamRank Has Done"));
	//calc Player Rank in Total and Team
	int TotalPlayerCount = 0; //mem total Player number
	TMap<int,int> TeamPlayerCount;//mem Player number in every team
	TMap<int,TMap<int,int>> PlayerScoreByTeam;//mem Player Score Rank in Team
	TMap<int,int> PlayerScoreByTotal;//mem Player Score Rank in total
	TMap<int,FString> TeamMVP;//mem Team's MVP
	TMap<int,TArray<int>> PlayerScoreByTeam_SortArray;

	for(APlayerState* PlayerState : PlayerArray)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			TotalPlayerCount++;
			SortArray.Add(DemoPlayerState->SelfScore);
			if(TeamPlayerCount.Contains(DemoPlayerState->TeamID)) TeamPlayerCount[DemoPlayerState->TeamID]++;
			else TeamPlayerCount.Add(DemoPlayerState->TeamID,1);
			PlayerScoreByTotal.FindOrAdd(DemoPlayerState->SelfScore,-1);
			if(PlayerScoreByTeam.Contains(DemoPlayerState->TeamID)==false)
			{
				TMap<int,int> RankinTeam;
				PlayerScoreByTeam.Add(DemoPlayerState->TeamID,RankinTeam);
			}
			PlayerScoreByTeam[DemoPlayerState->TeamID].FindOrAdd(DemoPlayerState->SelfScore,-1);
			if(PlayerScoreByTeam_SortArray.Contains(DemoPlayerState->TeamID)==false)
			{
				TArray<int> RankinTeam_SortArray;
				PlayerScoreByTeam_SortArray.Add(DemoPlayerState->TeamID,RankinTeam_SortArray);
			}
			PlayerScoreByTeam_SortArray[DemoPlayerState->TeamID].Add(DemoPlayerState->SelfScore);
		}
	}

	SortArray.Sort();
	RankIndex = TotalPlayerCount;
	for(int Score : SortArray)
	{
		PlayerScoreByTotal[Score] = RankIndex;
		RankIndex--;
	}
	for(TTuple<int, TArray<int>> Iterator : PlayerScoreByTeam_SortArray)
	{
		Iterator.Value.Sort();
		RankIndex = Iterator.Value.Num();
		for(int Score : Iterator.Value)
		{
			PlayerScoreByTeam[Iterator.Key][Score] = RankIndex;
			RankIndex--;
		}
	}
	
	for(APlayerState* PlayerState : PlayerArray)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			if(PlayerScoreByTeam[DemoPlayerState->TeamID][DemoPlayerState->SelfScore]==1)
			{
				if(TeamMVP.Contains(DemoPlayerState->TeamID)) TeamMVP[DemoPlayerState->TeamID] += ("/"+DemoPlayerState->Name);
				else TeamMVP.Add(DemoPlayerState->TeamID,DemoPlayerState->Name);
			}
		}
	}
	UE_LOG(LogGameState,Log,TEXT("Player Rank Has Done"));
	for(APlayerState* PlayerState : PlayerArray)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			FEndInfo EndInfo;
			EndInfo.bIsValid = true;
			EndInfo.SelfScore = DemoPlayerState->SelfScore;
			EndInfo.TotalTime = LastTime;
			EndInfo.bIsVictoryTeam = TeamSortIndex[ScoreMap[DemoPlayerState->TeamID]]==1 ? true : false;
			EndInfo.PlayerTeamCount = TeamPlayerCount[DemoPlayerState->TeamID];
			EndInfo.PlayerTeamScore = ScoreMap[DemoPlayerState->TeamID];
			EndInfo.SelfTeamRank = PlayerScoreByTeam[DemoPlayerState->TeamID][DemoPlayerState->SelfScore];
			EndInfo.SelfTotalRank = PlayerScoreByTotal[DemoPlayerState->SelfScore];
			EndInfo.TotalTeamCount = TotalTeamCount;
			EndInfo.TotalTeamRank = TeamSortIndex[ScoreMap[DemoPlayerState->TeamID]];
			EndInfo.VictoryTeamCount = TeamPlayerCount[VictoryTeam];
			EndInfo.VictoryTeamScore = ScoreMap[VictoryTeam];
			EndInfo.PlayerTeamID = DemoPlayerState->TeamID;
			EndInfo.VictoryMVP = TeamMVP[VictoryTeam];
			EndInfo.VictoryTeamID = VictoryTeam;
			EndInfo.PlayerTeamMVP = TeamMVP[DemoPlayerState->TeamID];
			EndInfo.TotalCount = TotalPlayerCount;

			DemoPlayerState->EndInfo = EndInfo;
			DemoPlayerState->GameEndDelegate.Broadcast();
		}
	}
}

bool ADemoGameStateBase::CheckNeedExtraTime()
{
	int MaxScore = -1;
	for(int Score : ScoreMap)
	{
		if(Score>MaxScore) MaxScore = Score;
	}
	int Count = 0;
	for(int Score : ScoreMap)
	{
		if(Score==MaxScore) Count++;
	}
	if(Count==1) return false;
	else return true;
}
