// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.generated.h"

UENUM()
enum EQuestEvent
{
	Collect,
	Travel,
	Kill,
	Interact
};

USTRUCT(Atomic, BlueprintType)
struct FQuestObjective : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EQuestEvent> QuestType;
};

USTRUCT(Atomic, BlueprintType)
struct FQuestStage : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestObjective> Objectives;
};

USTRUCT(Atomic, BlueprintType)
struct FQuestDetail : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestStage> Stages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
