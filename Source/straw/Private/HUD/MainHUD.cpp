// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"
#include "HUD/OrnamentOverlay.h"

UQuestOverlay* AMainHUD::GetQuestOverlay() const
{
	return QuestOverlay;
}

UOrnamentOverlay* AMainHUD::GetOrnamentOverlay() const
{
	return OrnamentOverlay;
}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			QuestOverlay = CreateWidget<UQuestOverlay>(PlayerController, QuestOverlayClass);
			QuestOverlay->AddToViewport();

			OrnamentOverlay = CreateWidget<UOrnamentOverlay>(PlayerController, OrnamentOverlayClass);
			OrnamentOverlay->AddToViewport();
		}
	}
}
