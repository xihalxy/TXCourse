// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlFile.h"
#include "UObject/NoExportTypes.h"
#include "RandomAwardBoxFactory.generated.h"

/**
 * 
 */
USTRUCT()
struct FAwardBoxStruct
{
	GENERATED_BODY()
	
	int AwardType;

	int Healthy;

	FString ObjectName;

	int Capacity;

	int Damage;//对武器的损耗

	int Weight;

	int Order;//第一第二第三武器
};
UCLASS(BlueprintType,Blueprintable)
class TXCOURSE_API URandomAwardBoxFactory : public UObject
{
	GENERATED_BODY()


public:

	void SpawnAwardBox();
	
	//UFUNCTION(BlueprintCallable)
	void SpawnAwardBox(FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void SpawnAwardBox(FTransform Transform,FString BoxName);

private:

	URandomAwardBoxFactory();

	void ReadXml();

	TArray<FAwardBoxStruct*> AwardBoxInfos;

	TMap<FString,int> BoxNameIndex;
	
	
};
