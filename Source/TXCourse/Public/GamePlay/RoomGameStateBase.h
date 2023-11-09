// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GamePlay/UI/RoomPlayerItemData.h"
#include "RoomGameStateBase.generated.h"


class ADemoPlayerState;
class URoomPlayerItemData;
/**
 * 
 */
UCLASS()
class TXCOURSE_API ARoomGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ARoomGameStateBase();

public:
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<FRoomItemStruct> GetRoomItemList();

	UFUNCTION(BlueprintCallable)
	void ChangePlayerTeamID(int DemoID,int TeamID);

	UFUNCTION(BlueprintCallable)
	void ChangePlayerName(int DemoID,const FString& NewName);

	UFUNCTION(BlueprintCallable)
	void ChangePlayerReady(int DemoID,bool bIsReady);

private:

	UPROPERTY(Replicated)
	int Count;
	
	UPROPERTY(Replicated)
	TArray<bool> bIsReadyMap;

	UPROPERTY(ReplicatedUsing=OnRep_bIsGoingToStart)
	bool bIsGoingToStart;
	
	UPROPERTY(ReplicatedUsing=OnRep_RoomPlayerItemList)
	TArray<FRoomItemStruct> RoomPlayerItemList;

	FTimerHandle StartCountDownHandle;

	UFUNCTION()
	void OnRep_bIsGoingToStart();
	
	UFUNCTION()
	void OnRep_RoomPlayerItemList();

	void SwitchStartMap();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
