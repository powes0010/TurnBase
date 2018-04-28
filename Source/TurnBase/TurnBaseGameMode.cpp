// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TurnBaseGameMode.h"
#include "Controller/TurnBasePlayerController.h"
#include "Character/TurnBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATurnBaseGameMode::ATurnBaseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATurnBasePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}