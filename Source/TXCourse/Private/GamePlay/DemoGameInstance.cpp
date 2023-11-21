// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DemoGameInstance.h"

void UDemoGameInstance::ConnectToServer(FString IpAddress)
{
	GetFirstLocalPlayerController()->ClientTravel(IpAddress,TRAVEL_Absolute);
}

