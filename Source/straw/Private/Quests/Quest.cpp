// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/Quest.h"

FQuest::FQuest()
{
	Initialize();
}

FQuest::FQuest(FQuestDetail InQuestDetail)
{
	Initialize();
	QuestDetail = InQuestDetail;
}

FQuest::~FQuest()
{
}

void FQuest::Initialize()
{
	CurrentStageIndex = 0;
	CurrentQuantity = 0;
}

void FQuest::Update(EQuestEvent QuestEvent, FString TargetID)
{
	FQuestStage CurrentStage = QuestDetail.Stages[CurrentStageIndex];
	FQuestObjective CurrentObjective = CurrentStage.Objective;

	if (CurrentObjective.QuestType != QuestEvent)
	{
		return;
	}

	if (!CurrentObjective.ObjectiveID.Equals(TargetID))
	{
		return;
	}

	if (++CurrentQuantity >= CurrentObjective.Quantity)
	{
		CompleteStage();
	}
}

const FQuestDetail& FQuest::GetQuestDetail() const
{
	return QuestDetail;
}

EQuestEvent FQuest::GetQuestType() const
{
	return GetCurrentStage().Objective.QuestType;
}

FQuestStage FQuest::GetCurrentStage() const
{
	return QuestDetail.Stages[CurrentStageIndex];
}

int32 FQuest::GetCurrentStageIndex() const
{
	return CurrentStageIndex;
}

int32 FQuest::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

bool FQuest::IsCompleted() const
{
	return bIsCompleted;
}

void FQuest::CompleteStage()
{
	if (++CurrentStageIndex >= QuestDetail.Stages.Num())
	{
		bIsCompleted = true;

		// QuestSubsystem의 EndQuest 호출 필요
	}
}

