// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterAttributeSet.h"
#include "InputMappingContext.h"
#include "Abilities/GameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Package/CharacterPackage.h"

#include "PlayerCharacter.generated.h"

class AWeaponBase;
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Stand,
	Crouch,
	Prone
};

UENUM(BlueprintType)
enum class EArmedWeaponType : uint8
{
	None,
	Sword,
	Gun,
	Magic
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeEvent,float,NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangeEvent,float,NewEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS()
class TXCOURSE_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
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

	//GAS System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable,Category="AbilitySystem")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);
	

	//Delegate Bind
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	
	void OnEnergyAttributeChanged(const FOnAttributeChangeData& Data);
	
	//Package
	void SwitchEquipment(UPackageItem* PackageItem,bool Enable);

	//RPC

	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const;
	
	//other

	UFUNCTION(Server,Reliable,BlueprintCallable,Category="CharacterState")
	void ChangeWeapon(EArmedWeaponType NewWeaponType,FName WeaponName);

	FRotator GetAimRotator();

	UFUNCTION(BlueprintCallable,Category="CharacterState")
	void SetMovementMaxSpeed(float Speed);
public:

	//input
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="EnhancedInput|Action", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Crouch;
	
	//Character State
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="CharacterState")
	ECharacterState CharacterState;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Replicated,Category="CharacterState")
	EArmedWeaponType WeaponType;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="CharacterState")
	bool bIsArmed;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="CharacterState")
	bool bIsDefence;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="CharacterState",Replicated)
	FRotator AimRotator;

	bool bIsDeadth;
	
	//GAS System
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AbilitySystem")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AbilitySystem")
	UCharacterAttributeSet* AttributeSet;

	//Delegate Bind
	UPROPERTY(BlueprintAssignable,Category="Ability")
	FOnHealthChangeEvent HealthChangeEvent;

	UPROPERTY(BlueprintAssignable,Category="Ability")
	FOnEnergyChangeEvent EnergyChangeEvent;

	UPROPERTY(BlueprintAssignable,Category="CharacterState")
	FDeathEvent DeathEvent;

	//Component
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(BlueprintReadOnly,Replicated)
	AWeaponBase* weapon_r;

	AWeaponBase* weapon_l;

	//Package
	UPROPERTY(BlueprintReadWrite,Category="CharacterPackage")
	UCharacterPackage* Package;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName FirstWeapon;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName SecondWeapon;
	
	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName ThirdWeapon;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName HeadEquipment;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName UpperEquipment;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName DownEquipment;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName FirstSkill;

	UPROPERTY(BlueprintReadOnly,Category="CharacterPackage")
	FName SecondSkill;

	//other
	UPROPERTY(BlueprintReadWrite,Category="CharacterScore")
	APlayerState* LastHitActorPlayerState;

private:

	void MoveForward(const FInputActionValue& InputValue);

	void MoveRight(const FInputActionValue& InputValue);

	void Turn(const FInputActionValue& InputValue);

	void LookUp(const FInputActionValue& InputValue);
	
	void SwitchCrouch(const FInputActionValue& InputValue);

	//StateFunction
	void EnergyRecover();

	void Dying();

	UFUNCTION(Server,Reliable)
	void SetAimRotator(FRotator NewRotator);
	
	//TimerHandle
	FTimerHandle EnergyRecoverTimer;
	
	void DebugRole();

	

	
};
