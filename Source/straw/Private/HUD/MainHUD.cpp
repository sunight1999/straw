// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"

UQuestOverlay* AMainHUD::GetQuestOverlay() const
{
	return QuestOverlay;
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
		}
	}
}
