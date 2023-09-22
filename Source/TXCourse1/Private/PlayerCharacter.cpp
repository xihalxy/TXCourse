// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	SpringArmComponent->TargetArmLength = 350.f;
	SpringArmComponent->AddRelativeRotation(FRotator(-45,0,0));
	if(CameraComponent!=nullptr && SpringArmComponent!=nullptr)
	{
		CameraComponent->SetupAttachment(SpringArmComponent);
		SpringArmComponent->SetupAttachment(GetRootComponent());
	}

	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	}

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
		AddMovementInput(Direction,InputValue.GetMagnitude());
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
		AddMovementInput(Direction,InputValue.GetMagnitude());
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
			//SpringArmComponent->AddRelativeRotation(FRotator(InputValue.GetMagnitude(),0,0));
		}
		//AddControllerPitchInput(InputValue.GetMagnitude());
	}
}

