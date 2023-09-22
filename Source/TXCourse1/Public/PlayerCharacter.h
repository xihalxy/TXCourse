// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TXCOURSE1_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="EnhancedInput",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Turn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_LookUp;
	
	UCameraComponent* CameraComponent;

	USpringArmComponent* SpringArmComponent;

private:

	void MoveForward(const FInputActionValue& InputValue);

	void MoveRight(const FInputActionValue& InputValue);

	void Turn(const FInputActionValue& InputValue);

	void LookUp(const FInputActionValue& InputValue);

};
