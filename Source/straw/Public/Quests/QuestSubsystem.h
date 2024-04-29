// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UQuestOverlay;
class FQuest;
enum EQuestEvent;

/**
 * 
 */
UCLASS()
class STRAW_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void AddQuest(FQuest* NewQuest);
	void EndQuest(FQuest* TargetQuest);
	void HandleEvent(EQuestEvent QuestEvent, FString TargetID);
	void UpdateUI();

protected:

private:
	void FetchOverlay();

	UQuestOverlay* QuestOverlay;
	TArray<FQuest*> CurrentQuests;
};
