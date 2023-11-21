// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamePlay/UI/RoomPlayerItemData.h"
#include "DemoPlayerState.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateRoomWidgetDelegate,const TArray<FRoomItemStruct>&,RoomPlayerList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountDownDelegate,bool,bNeedCountDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChangeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEndDelegate);

USTRUCT(BlueprintType)
struct FEndInfo
{

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid;
	
	UPROPERTY(BlueprintReadOnly)
	int VictoryTeamID;

	UPROPERTY(BlueprintReadOnly)
	int VictoryTeamScore;

	UPROPERTY(BlueprintReadOnly)
	int VictoryTeamCount;

	UPROPERTY(BlueprintReadOnly)
	FString VictoryMVP;

	UPROPERTY(BlueprintReadOnly)
	int PlayerTeamID;

	UPROPERTY(BlueprintReadOnly)
	int PlayerTeamScore;
	
	UPROPERTY(BlueprintReadOnly)
	int PlayerTeamCount;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerTeamMVP;

	UPROPERTY(BlueprintReadOnly)
	bool bIsVictoryTeam;

	UPROPERTY(BlueprintReadOnly)
	int SelfScore;

	UPROPERTY(BlueprintReadOnly)
	int SelfTeamRank;

	UPROPERTY(BlueprintReadOnly)
	int SelfTotalRank;

	UPROPERTY(BlueprintReadOnly)
	int TotalTeamRank;
	
	UPROPERTY(BlueprintReadOnly)
	int TotalTeamCount;
	
	UPROPERTY(BlueprintReadOnly)
	int TotalTime;

	UPROPERTY(BlueprintReadOnly)
	int TotalCount;

	FEndInfo(): bIsValid(false),
				VictoryTeamID(0),
				VictoryTeamScore(0),
				VictoryTeamCount(0),
				PlayerTeamID(0),
				PlayerTeamScore(0),
				PlayerTeamCount(0),
				bIsVictoryTeam(false),
				SelfScore(0),
				SelfTeamRank(0),
				SelfTotalRank(0),
				TotalTeamRank(0),
				TotalTeamCount(0),
				TotalTime(0),
				TotalCount(0)
	{
	}
};

UCLASS()
class TXCOURSE_API ADemoPlayerState : public APlayerState
{
	GENERATED_BODY()

	ADemoPlayerState();
public:

	UFUNCTION(BlueprintCallable)
	void SetTeamID(int ID);

	UFUNCTION(BlueprintCallable)
	int GetTeamID();

	UFUNCTION(BlueprintCallable)
	void SetName(const FString& NewName);

	UFUNCTION(BlueprintCallable)
	void SetReady(bool bIsReady);

	UFUNCTION(BlueprintCallable)
	void UpdateWidget(TArray<FRoomItemStruct> RoomPlayerList);

	virtual void CopyProperties(APlayerState* PlayerState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnScoreChange();

	UFUNCTION()
	void OnRep_EndInfo();
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnScoreChange)
	int SelfScore;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnScoreChange)
	int TeamScore;

	UPROPERTY(Replicated)
	int Rank;
	
	UPROPERTY(Replicated)
	int TeamID;

	UPROPERTY(Replicated)
	FString Name;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_EndInfo)
	FEndInfo EndInfo;

	UPROPERTY(BlueprintAssignable)
	FOnGameEndDelegate GameEndDelegate;

	UPROPERTY(BlueprintAssignable)
	FUpdateRoomWidgetDelegate UpdateRoomWidgetEvent;

	UPROPERTY(BlueprintAssignable)
	FCountDownDelegate CountDownEvent;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChangeDelegate ScoreChangeEvent;

	UPROPERTY(BlueprintReadOnly,Replicated)
	int DemoID;
	
};
