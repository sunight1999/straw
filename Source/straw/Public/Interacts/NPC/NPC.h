// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/BasicInteraction.h"
#include "Interacts/Interactable.h"
#include "NPC.generated.h"

class UQuestSubsystem;
class UDialogueSubsystem;
struct FQuestDetail;
struct FDialogue;

/**
 * 
 */
UCLASS()
class STRAW_API ANPC : public ABasicInteraction, public IInteractable
{
	GENERATED_BODY()
	
public:
	ANPC();

	void Interact() override;
	void Talk();
	void GiveQuest();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "NPC")
	USkeletalMeshComponent* SkeletalMesh; // 스켈레탈 메쉬로 변경 예정

	UPROPERTY(EditAnywhere, Category = "NPC")
	TArray<FString> DialogueIDs;

	UQuestSubsystem* QuestSubsystem;
	UDialogueSubsystem* DialogueSubsystem;

	FDialogue* CurrentDialogue;
};
