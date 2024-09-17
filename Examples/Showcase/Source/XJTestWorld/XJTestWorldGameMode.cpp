// Copyright Epic Games, Inc. All Rights Reserved.

#include "XJTestWorldGameMode.h"
#include "XJTestWorldPlayerController.h"
#include "XJTestWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXJTestWorldGameMode::AXJTestWorldGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AXJTestWorldPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}