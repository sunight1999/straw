// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubQuestUI.h"
#include "Components/TextBlock.h"
#include "Quests/Quest.h"

void USubQuestUI::SetQuest(FQuest* Quest)
{
	SubQuest = Quest;
	Update();
}

void USubQuestUI::Update()
{
	const FQuestDetail* QuestDetail = SubQuest->GetQuestDetail();
	if (QuestDetail)
	{
		SubQuestDescText->SetText(QuestDetail->Description);
		SubQuestQuantityText->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"),
			SubQuest->GetCurrentQuantity(),
			SubQuest->GetCurrentStage().Objectives[0].Quantity)));
	}
}
