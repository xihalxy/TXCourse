// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/UI/RoomPlayerItemData.h"


void URoomPlayerItemData::InitData(FRoomItemStruct RoomItemStruct)
{
	Name = RoomItemStruct.Name;
	TeamID = RoomItemStruct.TeamID;
}
