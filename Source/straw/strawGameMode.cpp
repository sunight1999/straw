// Copyright Epic Games, Inc. All Rights Reserved.

#include "strawGameMode.h"
#include "strawCharacter.h"
#include "UObject/ConstructorHelpers.h"

AstrawGameMode::AstrawGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Conent/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
