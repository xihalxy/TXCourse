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
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnScoreChange)
	int SelfScore;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnScoreChange)
	int TeamScore;
	
	UPROPERTY(Replicated)
	int TeamID;

	UPROPERTY(Replicated)
	FString Name;

	UPROPERTY(BlueprintAssignable)
	FUpdateRoomWidgetDelegate UpdateRoomWidgetEvent;

	UPROPERTY(BlueprintAssignable)
	FCountDownDelegate CountDownEvent;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChangeDelegate ScoreChangeEvent;

	UPROPERTY(BlueprintReadOnly,Replicated)
	int DemoID;

	UFUNCTION(BlueprintCallable)
	void Debug(FString String)
	{
		UE_LOG(LogTemp,Log,TEXT("%s"),*String);
	}
};
