// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/MyPlayerController.h"
#include "PlayerCharacter.h"
#include "GameFramework/TouchInterface.h"


AMyPlayerController::AMyPlayerController()
{
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

void AMyPlayerController::SetView(FVector ScreenDistance,float Rate)
{
	ScreenDistance.Z = 0;
	float length = ScreenDistance.Length();
	ScreenDistance.Normalize();
	float YawRate = ScreenDistance.X*Rate*length;
	float PitchRate = ScreenDistance.Y*Rate*length*2;
	AddYawInput(YawRate);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
	if(PlayerCharacter)
	{
		float Pitch = FMath::Clamp(PlayerCharacter->SpringArmComponent->GetRelativeRotation().Pitch+PitchRate,-45.f,45.f);
		FRotator Rotation = PlayerCharacter->SpringArmComponent->GetRelativeRotation();
		Rotation.Pitch = Pitch;
		PlayerCharacter->SpringArmComponent->SetRelativeRotation(Rotation);
	}
}
