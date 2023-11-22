// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemoGameStateBase.generated.h"
class APlayerCharacter;
class ADemoPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeChangeDelegate,int,NewTime);
/**
 * 
 */
UCLASS()
class TXCOURSE_API ADemoGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ADemoGameStateBase();
public:
	
	virtual void BeginPlay() override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	TArray<int> ScoreMap;

	void NotifyTeamScoreChange(int TeamID);
	
	void StartGame();

	void EndGame();

	void ClearGameTimer();

	float GetGameTime();

	void CheckSingleGame();

	UPROPERTY(BlueprintAssignable)
	FTimeChangeDelegate TimeChangeDelegate;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void PlayerReset(ACharacter* Character);

	UFUNCTION(Server,Reliable)
	void PlayerReBirth(APlayerController* PlayerController);

	int SingleModeRule;
	
private:
	
	FTimerHandle TimeCountHandle;

	FTimerHandle TotalGameTimer;
	
	int StartTime;

	UPROPERTY(ReplicatedUsing=OnRep_LastTime)
	int LastTime;

	int MaxTeam;
	
	void UpdateGameTime();

	void CalcPlayerRank();

	UFUNCTION()
	void OnRep_LastTime();
	
	bool CheckNeedExtraTime();

	bool bIsSingleTeam;
	
};
