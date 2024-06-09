// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"
#include "HUD/OrnamentOverlay.h"
#include "UI/DialogueUI.h"
#include "UI/SubGame/LockCheckUI.h"

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

ULockCheckUI* AMainHUD::GetLockCheckUI() const
{
	return LockCheckUI;
}

void AMainHUD::SetCrosshairVisible(bool bVisible)
{
	if (bVisible)
	{
		CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	}
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

			LockCheckUI = CreateWidget<ULockCheckUI>(PlayerController, LockCheckUIClass);
			LockCheckUI->AddToViewport();
			LockCheckUI->SetVisibility(ESlateVisibility::Hidden);

			CrosshairUI = CreateWidget<UUserWidget>(PlayerController, CrosshairUIClass);
			CrosshairUI->AddToViewport();
			CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
