// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QuestOverlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Quests/Quest.h"

void UQuestOverlay::SetQuest(FQuest* Quest)
{
	const FQuestDetail QuestDetail = Quest->GetQuestDetail();

	QuestTitleText->SetText(QuestDetail.Name);
	QuestDescText->SetText(QuestDetail.Description);
	QuestQuantityText->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"),
		Quest->GetCurrentQuantity(),
		Quest->GetCurrentStage().Objectives[0].Quantity)));
}
