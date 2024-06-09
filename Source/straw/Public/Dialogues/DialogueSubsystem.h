// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueSubsystem.generated.h"

class UDialogueUI;

using FDialogueLineDelegate = TDelegate<void()>;

USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayer;
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueLine> Lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestID;
};

/**
 * 
 */
UCLASS()
class STRAW_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDialogueSubsystem();

	FDialogue* StartDialogue(FString Name, FName DialogueID);

	template<class UserClass>
	FORCEINLINE FDialogue* StartDialogue(FString Name, FName DialogueID, UserClass* Object, typename FDialogueLineDelegate::TMethodPtr<UserClass> DialogueCallback);

	void EndDialogue();

	void Next();

	bool IsDisplaying() const { return bIsDisplaying; };

private:
	void FetchUI();

	UDialogueUI* DialogueWidget;

	UDataTable* DialogueDataTable;
	
	bool bIsDisplaying = false;
	FDialogue* CurrentDialogue;
	FDialogueLineDelegate CurrentDialogueDelegate;
	int CurrentLineIndex = 0;
	FString CurrentNPCName = "";
};

template<class UserClass>
FORCEINLINE FDialogue* UDialogueSubsystem::StartDialogue(FString Name, FName DialogueID, UserClass* Object, typename FDialogueLineDelegate::TMethodPtr<UserClass> DialogueCallback)
{
	if (bIsDisplaying)
	{
		return nullptr;
	}

	FDialogue* Dialogue = StartDialogue(Name, DialogueID);
	
	CurrentDialogueDelegate.Unbind();
	CurrentDialogueDelegate.BindUObject(Object, DialogueCallback);

	return Dialogue;
}