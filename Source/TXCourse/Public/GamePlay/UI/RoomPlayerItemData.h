// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomPlayerItemData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRoomItemStruct
{
	GENERATED_BODY()
public:
	UPROPERTY() FText Name;
	UPROPERTY() int TeamID;
};

UCLASS(Blueprintable)
class TXCOURSE_API URoomPlayerItemData : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void InitData(FRoomItemStruct RoomItemStruct);

	UPROPERTY(BlueprintReadOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly)
	int TeamID;
};
