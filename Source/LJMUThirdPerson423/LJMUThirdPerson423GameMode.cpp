// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LJMUThirdPerson423GameMode.h"
#include "LJMUThirdPerson423Character.h"
#include "UObject/ConstructorHelpers.h"

ALJMUThirdPerson423GameMode::ALJMUThirdPerson423GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
