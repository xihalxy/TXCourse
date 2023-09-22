// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/MyGameMode.h"

#include "GamePlay/MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
