// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LJMUThirdPerson423Character.generated.h"

UCLASS(config=Game)
class ALJMUThirdPerson423Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ALJMUThirdPerson423Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	virtual void Tick(float DeltaSeconds) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	//////////////
	//Double Jump
	virtual void Landed(const FHitResult& Hit) override;
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	///////////////////////////////////////
	//Sprint + Shift + Double Jump

	UFUNCTION()
		void DoubleJump();


	UPROPERTY()
		int DoubleJumpCounter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float Jumpheight;

	/////////
	UFUNCTION()
		void Sprint();

	UFUNCTION()
		void Walk();

	UPROPERTY(EditAnywhere)
		float WalkSpeed;

	UPROPERTY(EditAnywhere)
		float RunSpeed;
	////////////////////
	UFUNCTION()
		void Shift();

	UPROPERTY(EditAnywhere)
		float ShiftDistance;

	UPROPERTY(EditAnywhere)
		float ShiftCD;
	UPROPERTY(EditAnywhere)
		int ShitfMana;

	UPROPERTY()
		bool canShift;

	UPROPERTY(EditAnywhere)
		float ShiftStop;

	/////////////////////
	UPROPERTY()
		FTimerHandle Unusehandler;

	UFUNCTION()
		void StopShifting();

	UFUNCTION()
		void ResetShift();

	////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Intelligent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int nextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int curLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int curEXP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Maxmana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Curmana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurHealth;

	UFUNCTION()
		void LevelUP();

	UFUNCTION()
		void CheckLevel();

	UFUNCTION()
		void RestoreMana();

	UFUNCTION()
		void RestoreHealth();

	UFUNCTION()
		void LevelUP_DK();


};

