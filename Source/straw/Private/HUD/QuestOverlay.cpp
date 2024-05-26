// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QuestOverlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/HUD/SubQuestUI.h"
#include "Quests/Quest.h"

void UQuestOverlay::SetMainQuest(FQuest* Quest)
{
	MainQuest = Quest;
	Update();
}

void UQuestOverlay::AddSubQuest(FQuest* SubQuest)
{
	USubQuestUI* SubQuestUI = CreateWidget<USubQuestUI>(GetWorld(), USubQuestUI::StaticClass());
	SubQuestUI->SetQuest(SubQuest);

	SubQuestVBox->AddChildToVerticalBox(SubQuestUI);

	Update();
}

void UQuestOverlay::RemoveSubQuest(FQuest* SubQuest)
{
	for (int i = 0; i < SubQuestVBox->GetChildrenCount(); i++)
	{
		UWidget* Widget = SubQuestVBox->GetChildAt(i);
		USubQuestUI* SubQuestUI = Cast<USubQuestUI>(Widget);
		
		if (SubQuestUI && SubQuestUI->GetSubQuest() == SubQuest)
		{
			SubQuestVBox->RemoveChildAt(i);
			return;
		}
	}
}

void UQuestOverlay::Update()
{
	// 메인 퀘스트 정보 업데이트
	if (MainQuest)
	{
		const FQuestDetail* QuestDetail = MainQuest->GetQuestDetail();
		if (QuestDetail)
		{
			MainSymbolImage->SetVisibility(ESlateVisibility::Visible);
			MainQuestTitleText->SetText(QuestDetail->Name);
			MainQuestDescText->SetText(QuestDetail->Description);
			MainQuestQuantityText->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"),
				MainQuest->GetCurrentQuantity(),
				MainQuest->GetCurrentStage().Objectives[0].Quantity)));
		}
	}
	else
	{
		MainSymbolImage->SetVisibility(ESlateVisibility::Hidden);
		MainQuestTitleText->SetText(FText::GetEmpty());
		MainQuestDescText->SetText(FText::GetEmpty());
		MainQuestQuantityText->SetText(FText::GetEmpty());
	}

	// 서브 퀘스트 정보 업데이트
	for (auto Widget : SubQuestVBox->GetAllChildren())
	{
		USubQuestUI* SubQuestUI = Cast<USubQuestUI>(Widget);
		SubQuestUI->Update();
	}
}
