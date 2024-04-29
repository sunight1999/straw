// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestSubsystem.h"
#include "Characters/BaseCharacter.h"
#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"
#include "Quests/Quest.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubsystem::Deinitialize()
{
	
}

void UQuestSubsystem::AddQuest(FQuest* NewQuest)
{
	CurrentQuests.Add(NewQuest);
	UpdateUI();
}

void UQuestSubsystem::EndQuest(FQuest* TargetQuest)
{
	CurrentQuests.Remove(TargetQuest);
	UpdateUI();
}

void UQuestSubsystem::HandleEvent(EQuestEvent QuestEvent, FString TargetID)
{
	for (FQuest* Quest : CurrentQuests)
	{
		Quest->Update(QuestEvent, TargetID);
	}

	UpdateUI();
}

void UQuestSubsystem::UpdateUI()
{
	if (QuestOverlay == nullptr)
	{
		FetchOverlay();
	}

	// 현재 진행 중인 퀘스트를 모두 목록에 띄우도록 수정 필요
	if (CurrentQuests.Num() > 0)
	{
		QuestOverlay->SetQuest(CurrentQuests[0]);
	}
}

void UQuestSubsystem::FetchOverlay()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
	if (!MainHUD)
	{
		return;
	}

	QuestOverlay = MainHUD->GetQuestOverlay();
}
