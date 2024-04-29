// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum EQuestEvent
{
	Collect,
	Travel,
	Kill,
	Interact
};

struct FQuestObjective
{
public:
	FText Name;
	FText Description;
	FString ObjectiveID;
	int32 Quantity;
	EQuestEvent QuestType;
};

struct FQuestStage
{
public:
	FText Name;
	FText Description;
	FQuestObjective Objective;
};

struct FQuestDetail
{
public:
	FText Name;
	FText Description;
	TArray<FQuestStage> Stages;
	bool bIsMain;
};

/**
 * 
 */
class STRAW_API FQuest
{
public:
	FQuest();
	FQuest(FQuestDetail InQuestDetail);
	~FQuest();

	void Initialize();
	void Update(EQuestEvent QuestEvent, FString TargetID);

	const FQuestDetail& GetQuestDetail() const;
	EQuestEvent GetQuestType() const;
	FQuestStage GetCurrentStage() const;
	int32 GetCurrentStageIndex() const;
	int32 GetCurrentQuantity() const;

	bool IsCompleted() const;

private:
	void CompleteStage();

	FQuestDetail QuestDetail;
	int32 CurrentStageIndex;
	int32 CurrentQuantity;

	bool bIsCompleted;
};
