// Copyright Epic Games, Inc. All Rights Reserved.

#include "FCM_AlphaGameMode.h"
#include "FCM_AlphaPlayerController.h"
#include "FCM_AlphaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFCM_AlphaGameMode::AFCM_AlphaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFCM_AlphaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}