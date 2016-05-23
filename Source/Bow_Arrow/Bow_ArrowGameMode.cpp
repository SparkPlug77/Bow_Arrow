// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Bow_Arrow.h"
#include "Bow_ArrowGameMode.h"
#include "Bow_ArrowCharacter.h"
#include "PlayerHUD.h"

ABow_ArrowGameMode::ABow_ArrowGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = APlayerHUD::StaticClass();
}
