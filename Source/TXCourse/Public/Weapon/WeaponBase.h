// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.generated.h"

class UWeaponAttributeSet;
class UAbilitySystemComponent;
class APlayerCharacter;
UCLASS()
class TXCOURSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	// Sets default values for this actor's properties

	void InitWeapon(FString Name, int DamageValue, int HealthyValue,int CostValue,int CoolDownValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDestroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual APlayerCharacter* GetPlayerCharacter();

	virtual APlayerState* GetBelongPlayerState();

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void BindToPlayerCharacter(APlayerCharacter* PlayerCharacter);
	
	virtual void StartAttack();
	
	virtual void EndAttack();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Appearance)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AbilitySystem")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AbilitySystem")
	UWeaponAttributeSet* AttributeSet;

protected:

	UPROPERTY(Replicated)
	FString WeaponName;

	int Healthy;

	UPROPERTY(Replicated)
	APlayerState* BelongPlayerState;

	UPROPERTY(Replicated)
	APlayerCharacter* BelongPlayerCharacter;

};
