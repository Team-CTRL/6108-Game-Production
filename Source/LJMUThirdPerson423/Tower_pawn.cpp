// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower_pawn.h"

// Sets default values
ATower_pawn::ATower_pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATower_pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATower_pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	if (Overlap)
	{
		if (NewLocation.Z > 200.0f)
		{
			bGoingUp = false;
		}
		else if (NewLocation.Z < -200.0f)
		{
			bGoingUp = true;
		}
		float factor = (bGoingUp ? 200.0f : -200.0f);
		NewLocation.Z += factor * DeltaTime;
		SetActorLocation(NewLocation, 0, false, ETeleportType::None);
	}
	
}

// Called to bind functionality to input
void ATower_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATower_pawn::TowerRising()
{
	Overlap = true;
}

