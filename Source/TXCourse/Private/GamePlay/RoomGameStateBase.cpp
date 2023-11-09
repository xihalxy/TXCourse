// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/RoomGameStateBase.h"
#include "GamePlay/DemoPlayerState.h"
#include "Net/UnrealNetwork.h"

ARoomGameStateBase::ARoomGameStateBase()
{
	Count = 0;
	bIsReadyMap.Empty();
	
}

void ARoomGameStateBase::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if(GetLocalRole()==ROLE_Authority)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			DemoPlayerState->DemoID = Count;
			bIsReadyMap.Push(false);
			Count++;
			check(Count==bIsReadyMap.Num());
			FRoomItemStruct Item= {FText::FromString(DemoPlayerState->Name),DemoPlayerState->TeamID};
			RoomPlayerItemList.Add(Item);
			OnRep_RoomPlayerItemList();
		}
	}
}

void ARoomGameStateBase::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

TArray<FRoomItemStruct> ARoomGameStateBase::GetRoomItemList()
{
	return RoomPlayerItemList;
}

void ARoomGameStateBase::ChangePlayerTeamID(int DemoID,int TeamID)
{
	if(DemoID>=0&&DemoID<Count)
	{
		RoomPlayerItemList[DemoID].TeamID=TeamID;
		OnRep_RoomPlayerItemList();
	}
}


void ARoomGameStateBase::ChangePlayerName(int DemoID, const FString& NewName)
{
	if(DemoID>=0&&DemoID<Count)
	{
		RoomPlayerItemList[DemoID].Name = FText::FromString(NewName);
		OnRep_RoomPlayerItemList();
	}
}

void ARoomGameStateBase::ChangePlayerReady(int DemoID, bool bIsReady)
{
	if(DemoID>=0&&DemoID<Count)
	{
		bIsReadyMap[DemoID] = bIsReady;
		if(bIsReady)
		{
			bool CheckRes = true;
			for(bool flag:bIsReadyMap)
			{
				if(flag==false)
				{
					CheckRes = false;
					break;
				}
			}
			if(CheckRes && !bIsGoingToStart)
			{
				bIsGoingToStart = true;
				OnRep_bIsGoingToStart();
				GetWorldTimerManager().SetTimer(StartCountDownHandle,this,&ARoomGameStateBase::SwitchStartMap,5,false);
			}
		}else if(bIsGoingToStart)
		{
			bIsGoingToStart = false;
			GetWorldTimerManager().ClearTimer(StartCountDownHandle);
			OnRep_bIsGoingToStart();
		}
		
	}
}

void ARoomGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomGameStateBase,bIsReadyMap);
	DOREPLIFETIME(ARoomGameStateBase,Count);
	DOREPLIFETIME(ARoomGameStateBase,RoomPlayerItemList);
	DOREPLIFETIME(ARoomGameStateBase,bIsGoingToStart);
}

void ARoomGameStateBase::OnRep_bIsGoingToStart()
{
	for(APlayerState* PlayerState : PlayerArray)
	{
		if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
		{
			DemoPlayerState->CountDownEvent.Broadcast(bIsGoingToStart);
		}
	}
}

void ARoomGameStateBase::OnRep_RoomPlayerItemList()
{
	for(APlayerState* PlayerState : PlayerArray)
	{
		//if(PlayerState->GetLocalRole()==ROLE_AutonomousProxy||PlayerState->GetLocalRole()==ROLE_Authority)
		{
			if(ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
			{
				DemoPlayerState->UpdateRoomWidgetEvent.Broadcast(RoomPlayerItemList);
			}
			break;
		}
	}
}



void ARoomGameStateBase::SwitchStartMap()
{
	GWorld->ServerTravel("DemoMap4");
}


