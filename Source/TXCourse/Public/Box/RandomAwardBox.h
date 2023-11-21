// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomAwardBox.generated.h"
class APlayerCharacter;
UENUM()
enum class EAwardType
{
	None=0,
	Money,
	Skill,
	Weapon,
	Buff,
	DeBuff
};
UCLASS()
class TXCOURSE_API ARandomAwardBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomAwardBox();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Appearance)
	UStaticMeshComponent* StaticMeshComponent;
	
	//UGeometryCollectionComponent* GeometryCollectionComponent;
	
	/*UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void InitBox(int awardType, FString objectName, int capacity,int weight,int order);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EAwardType AwardType;

	FString ObjectName;

	int Capacity;

	int Weight;

	int Order;

	APlayerCharacter* LastCollisionActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	void OnDestroyed();
	

};
