// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Weapon/WeaponBase.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/DAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GamePlay/DemoPlayerState.h"
#include "GamePlay/MyGameMode.h"
#include "GamePlay/MyPlayerController.h"
#include "GamePlay/SingletonSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/NoneWeapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	bReplicates = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	bUseControllerRotationYaw = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	SpringArmComponent->TargetArmLength = 350.f;
	SpringArmComponent->AddRelativeRotation(FRotator(-45,0,0));
	if(CameraComponent!=nullptr && SpringArmComponent!=nullptr)
	{
		CameraComponent->SetupAttachment(SpringArmComponent);
		SpringArmComponent->SetupAttachment(GetRootComponent());
	}
	CameraComponent->SetRelativeLocation(FVector(50,50,50));
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Package=NewObject<UCharacterPackage>();
	Package->Character = this;

	WeaponType = EArmedWeaponType::None;
	
	weapon_r = Cast<AWeaponBase>(GWorld->SpawnActor(ANoneWeapon::StaticClass()));
	if(weapon_r)
	{
		weapon_r->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("weapon_no_r"));
		weapon_r->BindToPlayerCharacter(this);
	}
	weapon_l = nullptr;
	if(AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHealthAttribute())
		.AddUObject(this,&APlayerCharacter::OnHealthAttributeChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetEnergyAttribute())
		.AddUObject(this,&APlayerCharacter::OnEnergyAttributeChanged);
	}
	bIsDefence = false;

	FirstWeapon = "";
	SecondWeapon = "";
	ThirdWeapon = "";
	HeadEquipment = "";
	UpperEquipment = "";
	DownEquipment = "";
	FirstSkill = "";
	SecondSkill = "";

	LastHitActorPlayerState = nullptr;
	bIsDeadth = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if(PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext,100);
		}
	}
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
	{
		if(IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward,ETriggerEvent::Triggered,this,&APlayerCharacter::MoveForward);
		}
		if(IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight,ETriggerEvent::Triggered,this,&APlayerCharacter::MoveRight);
		}
		if(IA_Turn)
		{
			EnhancedInputComponent->BindAction(IA_Turn,ETriggerEvent::Triggered,this,&APlayerCharacter::Turn);
		}
		if(IA_LookUp)
		{
			EnhancedInputComponent->BindAction(IA_LookUp,ETriggerEvent::Triggered,this,&APlayerCharacter::LookUp);
		}
		if(IA_Crouch)
		{
			EnhancedInputComponent->BindAction(IA_Crouch,ETriggerEvent::Triggered,this,&APlayerCharacter::SwitchCrouch);
		}
	}

}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void APlayerCharacter::GiveAbility(TSubclassOf<UGameplayAbility> Ability)
{
	if(AbilitySystem)
	{
		if(HasAuthority() &&Ability)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability,1));
		}
		AbilitySystem->InitAbilityActorInfo(this,this);
	}
}

FRotator APlayerCharacter::GetAimRotator()
{
	return AimRotator;
}

void APlayerCharacter::SetMovementMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void APlayerCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	HealthChangeEvent.Broadcast(Data.NewValue);
	if(Data.NewValue<=0)
	{
		if(bIsDeadth) return;
		bIsDeadth = true;
		Dying();
		DeathEvent.Broadcast();
	}
}

void APlayerCharacter::OnEnergyAttributeChanged(const FOnAttributeChangeData& Data)
{
	EnergyChangeEvent.Broadcast(Data.NewValue);
	bool bSuccess;
	float MaxEnergy = UAbilitySystemBlueprintLibrary::GetFloatAttribute(this,UCharacterAttributeSet::GetMaxEnergyAttribute(),bSuccess);
	if(bSuccess&&MaxEnergy>Data.NewValue)
	{
		GWorld->GetTimerManager().SetTimer(EnergyRecoverTimer,this,&APlayerCharacter::EnergyRecover,1,true,-1);
	}
}


void APlayerCharacter::SwitchEquipment(UPackageItem* PackageItem,bool Enable)
{
	if(Enable)
	{
		switch (PackageItem->PackageItemType) {
			case EPackageItemType::MainWeapon:
				FirstWeapon = PackageItem->ItemName;
				if(WeaponType==EArmedWeaponType::Gun)
				{
					weapon_r->Destroy();
					weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(FirstWeapon.ToString(),FName("weapon_gun_r"),GetMesh());
					weapon_r->BindToPlayerCharacter(this);
				}
				break;
			case EPackageItemType::SecondWeapon:
				SecondWeapon = PackageItem->ItemName;
				if(WeaponType==EArmedWeaponType::Magic)
				{
					weapon_r->Destroy();
					weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(SecondWeapon.ToString(),FName("weapon_magicstick_r"),GetMesh());
					weapon_r->BindToPlayerCharacter(this);
				}
				break;
			case EPackageItemType::ThirdWeapon:
				ThirdWeapon = PackageItem->ItemName;
				if(WeaponType==EArmedWeaponType::Sword)
				{
					weapon_r->Destroy();
					weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(ThirdWeapon.ToString(),FName("weapon_sword_r"),GetMesh());
					weapon_r->BindToPlayerCharacter(this);
				}
				break;
			case EPackageItemType::HeadEquip:
				HeadEquipment = PackageItem->ItemName;
				break;
			case EPackageItemType::UpperEquip:
				UpperEquipment = PackageItem->ItemName;
				break;
			case EPackageItemType::DownEquip:
				DownEquipment = PackageItem->ItemName;
				break;
			case EPackageItemType::MainSkillBook:
				FirstSkill = PackageItem->ItemName;
				break;
			case EPackageItemType::SecondSkillBook:
				SecondSkill = PackageItem->ItemName;
				break;
			default:
				break;
		}
	}else
	{
		switch (PackageItem->PackageItemType) {
			case EPackageItemType::MainWeapon:
				FirstWeapon = "";
				break;
			case EPackageItemType::SecondWeapon:
				SecondWeapon = "";
				break;
			case EPackageItemType::ThirdWeapon:
				ThirdWeapon = "";
				break;
			case EPackageItemType::HeadEquip:
				HeadEquipment = "";
				break;
			case EPackageItemType::UpperEquip:
				UpperEquipment = "";
				break;
			case EPackageItemType::DownEquip:
				DownEquipment = "";
				break;
			case EPackageItemType::MainSkillBook:
				FirstSkill = "";
				break;
			case EPackageItemType::SecondSkillBook:
				SecondSkill = "";
				break;
			default:
				break;
		}
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( APlayerCharacter, weapon_r );
	DOREPLIFETIME( APlayerCharacter, WeaponType );
}

void APlayerCharacter::ChangeWeapon_Implementation(EArmedWeaponType NewWeaponType,FName WeaponName)
{
	if(NewWeaponType==WeaponType||WeaponName=="")
	{
		return;
	}
	switch (NewWeaponType) {
		case EArmedWeaponType::None:
			weapon_r->Destroy();
			weapon_r = Cast<AWeaponBase>(GWorld->SpawnActor(ANoneWeapon::StaticClass()));
			if(weapon_r)
			{
				weapon_r->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("weapon_no_r"));
			}
			break;
		case EArmedWeaponType::Sword:
			weapon_r->Destroy();
			weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(WeaponName.ToString(),FName("weapon_sword_r"),GetMesh());
			weapon_r->BindToPlayerCharacter(this);
			break;
		case EArmedWeaponType::Gun:
			weapon_r->Destroy();
			weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(WeaponName.ToString(),FName("weapon_gun_r"),GetMesh());
			weapon_r->BindToPlayerCharacter(this);
			break;
		case EArmedWeaponType::Magic:
			weapon_r->Destroy();
			weapon_r = GetGameInstance()->GetSubsystem<USingletonSubsystem>()->GetWeaponFactoryInst()->SpawnWeapon(WeaponName.ToString(),FName("weapon_magicstick_r"),GetMesh());
			weapon_r->BindToPlayerCharacter(this);
			break;
		default: ;
	}
	if(weapon_r)
	{
		weapon_r->BindToPlayerCharacter(this);
	}
	WeaponType = NewWeaponType;
}

void APlayerCharacter::MoveForward(const FInputActionValue& InputValue)
{
	if(Controller!=nullptr && InputValue.GetMagnitude() != 0.0f)
	{
		FRotator Rotation = Controller->GetControlRotation();
		UCharacterMovementComponent* CharacterMovementComponent =  GetCharacterMovement();
		if(CharacterMovementComponent->IsMovingOnGround()||CharacterMovementComponent->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		float Speed = InputValue.GetMagnitude();
		/*if(CharacterState==ECharacterState::Crouch)
		{
			Speed = FMath::Clamp(Speed,-0.5,0.5);
		}*/
		AddMovementInput(Direction,Speed);
	}
}

void APlayerCharacter::MoveRight(const FInputActionValue& InputValue)
{
	if(Controller!=nullptr && InputValue.GetMagnitude() != 0.0f)
	{
		FRotator Rotation = Controller->GetControlRotation();
		UCharacterMovementComponent* CharacterMovementComponent =  GetCharacterMovement();
		if(CharacterMovementComponent->IsMovingOnGround()||CharacterMovementComponent->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		float Speed = InputValue.GetMagnitude();
		/*if(CharacterState==ECharacterState::Crouch)
		{
			Speed = FMath::Clamp(Speed,-0.5,0.5);
			
		}*/
		AddMovementInput(Direction,Speed);
	}
}

void APlayerCharacter::Turn(const FInputActionValue& InputValue)
{
	if(Controller!=nullptr && InputValue.GetMagnitude() !=0.0f){
		AddControllerYawInput(InputValue.GetMagnitude());
	}
}

void APlayerCharacter::LookUp(const FInputActionValue& InputValue)
{
	if(Controller!=nullptr && InputValue.GetMagnitude() !=0.0f)
	{
		if(SpringArmComponent)
		{
			float Pitch = FMath::Clamp(SpringArmComponent->GetRelativeRotation().Pitch+InputValue.GetMagnitude(),-45.f,45.f);
			FRotator Rotation = SpringArmComponent->GetRelativeRotation();
			Rotation.Pitch = Pitch;
			SpringArmComponent->SetRelativeRotation(Rotation);
			SetAimRotator(CameraComponent->GetComponentRotation());
			//SpringArmComponent->AddRelativeRotation(FRotator(InputValue.GetMagnitude(),0,0));
		}
		//AddControllerPitchInput(InputValue.GetMagnitude());
	}
}

void APlayerCharacter::SwitchCrouch(const FInputActionValue& InputValue)
{
	if(CharacterState==ECharacterState::Crouch)
	{
		CharacterState=ECharacterState::Stand;
		bIsCrouched = false;
		SetMovementMaxSpeed(600);
	}else
	{
		CharacterState=ECharacterState::Crouch;
		bIsCrouched = true;
		SetMovementMaxSpeed(300);
	}
}

void APlayerCharacter::EnergyRecover()
{
	AbilitySystem->ApplyModToAttribute(UCharacterAttributeSet::GetEnergyAttribute(),EGameplayModOp::Additive,1);
}

void APlayerCharacter::Dying()
{
	//DebugRole();
	if(ADemoPlayerState* MarkPlayerState = Cast<ADemoPlayerState>(LastHitActorPlayerState))
	{
		if(GetPlayerState<ADemoPlayerState>() && MarkPlayerState->TeamID==GetPlayerState<ADemoPlayerState>()->TeamID) return;
		if(AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->AddScore(MarkPlayerState);
			MarkPlayerState->OnScoreChange();
		}
	}
	if(AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
	{
		PlayerController->ReBirth();
		if(AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->AddDeath();
		}
	}
	if(ADAIController* AIController = Cast<ADAIController>(GetController()))
	{
		AIController->Destroy();
	}
	if(weapon_r) weapon_r->Destroy();
}

void APlayerCharacter::SetAimRotator_Implementation(FRotator NewRotator)
{
	AimRotator = NewRotator;
}

void APlayerCharacter::DebugRole()
{
	switch (GetLocalRole()) {
	case ROLE_None:
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Yellow,FString("ROLE_None"));
		UE_LOG(LogTemp,Warning,TEXT("ROLE_None"));
		break;
	case ROLE_SimulatedProxy:
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Yellow,FString("ROLE_SimulatedProxy"));
		UE_LOG(LogTemp,Warning,TEXT("ROLE_SimulatedProxy"));
		break;
	case ROLE_AutonomousProxy:
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Yellow,FString("ROLE_AutonomousProxy"));
		UE_LOG(LogTemp,Warning,TEXT("ROLE_AutonomousProxy"));
		break;
	case ROLE_Authority:
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Yellow,FString("ROLE_Authority"));
		UE_LOG(LogTemp,Warning,TEXT("ROLE_Authority"));
		break;
	case ROLE_MAX:
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Yellow,FString("ROLE_MAX"));
		UE_LOG(LogTemp,Warning,TEXT("ROLE_MAX"));
		break;
	default: ;
	}
}
