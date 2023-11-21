// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/MyPlayerController.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/TouchInterface.h"
#include "GamePlay/DemoGameStateBase.h"


AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UTouchInterface>TouchInterface (TEXT("/Script/Engine.TouchInterface'/Game/TouchInterfaceHud/UserTouchHUD.UserTouchHUD'"));
	if(TouchInterface.Succeeded())
	{
		TouchInterfaceInst = TouchInterface.Object;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(TouchInterfaceInst)
	{
		ActivateTouchInterface(TouchInterfaceInst);
	}
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitTeamIDEvent.Broadcast();
}

void AMyPlayerController::SetView(FVector ScreenDistance,float Rate)
{
	ScreenDistance.Z = 0;
	float length = ScreenDistance.Length();
	ScreenDistance.Normalize();
	float YawRate = ScreenDistance.X*Rate*length;
	float PitchRate = ScreenDistance.Y*Rate*length*2;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
	AddYawInput(YawRate);
	if(PlayerCharacter)
	{
		float Pitch = FMath::Clamp(PlayerCharacter->SpringArmComponent->GetRelativeRotation().Pitch+PitchRate,-45.f,45.f);
		FRotator Rotation = PlayerCharacter->SpringArmComponent->GetRelativeRotation();
		Rotation.Pitch = Pitch;
		PlayerCharacter->SpringArmComponent->SetRelativeRotation(Rotation);
	}
}

void AMyPlayerController::ReBirth()
{
	DelayForce.Broadcast(5);
	//UE_LOG(LogTemp,Warning,TEXT("Rebirth"));
	auto Lambda = [](APlayerController* PlayerController)
	{
		GWorld->GetGameState<ADemoGameStateBase>()->PlayerReBirth(PlayerController);
	};
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(Lambda,this);
	SetPawn(nullptr);
	GetWorldTimerManager().SetTimer(TimerHandle,TimerDelegate,5,false);
}

void AMyPlayerController::ResetPlayer()
{
	DelayForce.Broadcast(2);

	auto Lambda = [this]()
	{
		GWorld->GetGameState<ADemoGameStateBase>()->PlayerReset(GetCharacter());
	};
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(Lambda);

	GetWorldTimerManager().SetTimer(TimerHandle,TimerDelegate,2,false);
}
