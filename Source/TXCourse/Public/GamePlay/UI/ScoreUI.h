// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamScoreChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelfScoreChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChange,float,NewTime);
/**
 * 
 */
UCLASS()
class TXCOURSE_API UScoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly,Category="Score")
	int TeamScore;
	
	UPROPERTY(BlueprintReadOnly,Category="Score")
	int SelfScore;

	UPROPERTY(BlueprintAssignable,Category="ScoreEvent")
	FOnTeamScoreChange TeamScoreChangeEvent;

	UPROPERTY(BlueprintAssignable,Category="ScoreEvent")
	FOnSelfScoreChange SelfScoreChangeEvent;

	UPROPERTY(BlueprintAssignable,Category="ScoreEvent")
	FOnScoreChange ScoreChangeEvent;

	UPROPERTY(BlueprintAssignable,Category="TimeEvent")
	FOnTimeChange TimeChangeEvent;

	UFUNCTION(BlueprintCallable,Category="ScoreEvent")
	void UpdateSelfScore(int NewScore);
	
	UFUNCTION(BlueprintCallable,Category="ScoreEvent")
	void UpdateTeamScore(int NewScore);
	
	UFUNCTION(BlueprintCallable,Category="ScoreEvent")
	void UpdateScore(int NewSelfScore, int NewTeamScore);

	UFUNCTION(BlueprintCallable,Category="Time")
	void InitSlate();

private:
	void UpdateTime();

	FTimerHandle TimeHandle;
};
