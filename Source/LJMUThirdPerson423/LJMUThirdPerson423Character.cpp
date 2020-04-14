// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LJMUThirdPerson423Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
/////-------------------
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
//////////////////////////////////////////////////////////////////////////
// ALJMUThirdPerson423Character

ALJMUThirdPerson423Character::ALJMUThirdPerson423Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/////////////////////
	Jumpheight = 600.0f;
	WalkSpeed = 600.0f;
	RunSpeed = 1000.0f;

	/////////////////////
	canShift = true;
	ShiftDistance = 6000.0f;
	ShiftCD = 2.0f;
	ShiftStop = 0.1f;
	ShitfMana = 20;
	
	/////////////////////////
	curLevel = 1;
	Health = 150;
	Maxmana = 150;
	Strength = 100;
	Intelligent = 25;
	nextLevel = 100;
	curEXP = 0;
	Curmana = Maxmana;
	CurHealth = Health;
}

void ALJMUThirdPerson423Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RestoreMana();
	RestoreHealth();
	CheckLevel();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALJMUThirdPerson423Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALJMUThirdPerson423Character::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	///----------------/////////////////
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALJMUThirdPerson423Character::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALJMUThirdPerson423Character::Walk);


	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &ALJMUThirdPerson423Character::Shift);


	PlayerInputComponent->BindAction("DK_LevelUP", IE_Pressed, this, &ALJMUThirdPerson423Character::LevelUP_DK);
	
	//-----------------------//////////////
	PlayerInputComponent->BindAxis("MoveForward", this, &ALJMUThirdPerson423Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALJMUThirdPerson423Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALJMUThirdPerson423Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALJMUThirdPerson423Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALJMUThirdPerson423Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALJMUThirdPerson423Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALJMUThirdPerson423Character::OnResetVR);
}

void ALJMUThirdPerson423Character::Landed(const FHitResult & Hit)
{
	DoubleJumpCounter = 0;
}

void ALJMUThirdPerson423Character::DoubleJump()
{
	if (DoubleJumpCounter <= 1)
	{
		ACharacter::LaunchCharacter(FVector(0, 0, Jumpheight), false, true);
		DoubleJumpCounter++;
	}
}

void ALJMUThirdPerson423Character::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ALJMUThirdPerson423Character::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ALJMUThirdPerson423Character::Shift()
{
	if (canShift && Curmana >= 0)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal()*ShiftDistance, true, true);
		Curmana = Curmana - 20;
		canShift = false;
		GetWorldTimerManager().SetTimer(Unusehandler, this, &ALJMUThirdPerson423Character::StopShifting, ShiftStop, false);
		UE_LOG(LogTemp, Warning, TEXT("mana - 20"));
	}
}

void ALJMUThirdPerson423Character::StopShifting()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(Unusehandler, this, &ALJMUThirdPerson423Character::ResetShift, ShiftCD, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
}

void ALJMUThirdPerson423Character::ResetShift()
{
	canShift = true;
}

void ALJMUThirdPerson423Character::LevelUP()
{
	curEXP = 0;
	nextLevel = nextLevel + 100;
	Health = Health + 50;
	Maxmana = Maxmana + 50;
	Strength = Strength + 50;
	Intelligent = Intelligent + 10;
}

void ALJMUThirdPerson423Character::CheckLevel()
{
	if (curEXP == nextLevel)
	{
		LevelUP();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("level UP Fail"));
	}
}

void ALJMUThirdPerson423Character::RestoreMana()
{
	if (Curmana != Maxmana)
	{
		Curmana = Maxmana * 0.02;
		UE_LOG(LogTemp, Warning, TEXT("ManaRegeneration"));
	}
}

void ALJMUThirdPerson423Character::RestoreHealth()
{
	if(CurHealth != Health)
	{
		CurHealth = Health * 0.02;
		UE_LOG(LogTemp, Warning, TEXT("HealthRegeneration"));
		
	}
}

void ALJMUThirdPerson423Character::LevelUP_DK()
{
	curEXP = curEXP + 99;
	UE_LOG(LogTemp, Warning, TEXT("EXP+100"));
}


void ALJMUThirdPerson423Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALJMUThirdPerson423Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALJMUThirdPerson423Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALJMUThirdPerson423Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALJMUThirdPerson423Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALJMUThirdPerson423Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALJMUThirdPerson423Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
