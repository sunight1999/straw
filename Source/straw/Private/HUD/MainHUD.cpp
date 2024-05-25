// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"
#include "HUD/OrnamentOverlay.h"
#include "UI/DialogueUI.h"

UQuestOverlay* AMainHUD::GetQuestOverlay() const
{
	return QuestOverlay;
}

UOrnamentOverlay* AMainHUD::GetOrnamentOverlay() const
{
	return OrnamentOverlay;
}

UDialogueUI* AMainHUD::GetDialogueUI() const
{
	return DialogueUI;
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
			QuestOverlay->SetVisibility(ESlateVisibility::Hidden);

			OrnamentOverlay = CreateWidget<UOrnamentOverlay>(PlayerController, OrnamentOverlayClass);
			OrnamentOverlay->AddToViewport();
			OrnamentOverlay->SetVisibility(ESlateVisibility::Hidden);

			DialogueUI = CreateWidget<UDialogueUI>(PlayerController, DialogueUIClass);
			DialogueUI->AddToViewport();
			DialogueUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
