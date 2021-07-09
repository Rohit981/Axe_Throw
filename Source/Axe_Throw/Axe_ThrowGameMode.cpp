// Copyright Epic Games, Inc. All Rights Reserved.

#include "Axe_ThrowGameMode.h"
#include "GameHUD.h"
#include "Axe_ThrowCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAxe_ThrowGameMode::AAxe_ThrowGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

		HUDClass = AGameHUD::StaticClass();
	}
}
