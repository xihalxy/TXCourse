// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DemoPlayerState.h"
#include "Gameplay/RoomGameStateBase.h"
#include "Net/UnrealNetwork.h"

ADemoPlayerState::ADemoPlayerState()
{
	SelfScore = 0;
	TeamScore = 0;
	TeamID = 0;
	DemoID = -1;
	Name = "Player"+FString::FromInt(GetPlayerId());
}

void ADemoPlayerState::SetTeamID(int ID)
{
	TeamID = ID;
	GetWorld()->GetGameState<ARoomGameStateBase>()->ChangePlayerTeamID(DemoID,TeamID);
}

int ADemoPlayerState::GetTeamID()
{
	return TeamID;
}

void ADemoPlayerState::SetName(const FString& NewName)
{
	Name = NewName;
	GetWorld()->GetGameState<ARoomGameStateBase>()->ChangePlayerName(DemoID,Name);
}

void ADemoPlayerState::SetReady(bool bIsReady)
{
	GetWorld()->GetGameState<ARoomGameStateBase>()->ChangePlayerReady(DemoID,bIsReady);
}

void ADemoPlayerState::UpdateWidget(TArray<FRoomItemStruct> RoomPlayerList)
{
	UpdateRoomWidgetEvent.Broadcast(RoomPlayerList);
}


void ADemoPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	ADemoPlayerState* NewPlayerState = Cast<ADemoPlayerState>(PlayerState);
	if(NewPlayerState)
	{
		NewPlayerState->TeamID = TeamID;
		NewPlayerState->Name = Name;
	}
}

void ADemoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoPlayerState,DemoID);
	DOREPLIFETIME(ADemoPlayerState,TeamID);
	DOREPLIFETIME(ADemoPlayerState,Name);
	DOREPLIFETIME(ADemoPlayerState,SelfScore);
	DOREPLIFETIME(ADemoPlayerState,TeamScore);
}

void ADemoPlayerState::OnScoreChange()
{
	ScoreChangeEvent.Broadcast();
}


