// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/NPC/NPC.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

#include "Quests/QuestSubsystem.h"
#include "Quests/Quest.h"
#include "Dialogues/DialogueSubsystem.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// 설정한 Mesh 크기에 맞춰 Interactable UI 위치 조정
	RelocationInteractableWidget(StaticMesh);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return;
	}

	// 퀘스트 서브시스템 로드
	QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>();
	if (!QuestSubsystem)
	{
		return;
	}

	// 다이얼로그 서브시스템 로드
	DialogueSubsystem = GameInstance->GetSubsystem<UDialogueSubsystem>();
	if (!DialogueSubsystem)
	{
		return;
	}
}

void ANPC::Interact()
{
	Talk();
}

void ANPC::Talk()
{
	if (DialogueIDs.Num() > 0)
	{
		CurrentDialogue = DialogueSubsystem->StartDialogue(FName(DialogueIDs[0]), this, &ANPC::GiveQuest);
	}
}

void ANPC::GiveQuest()
{
	if (CurrentDialogue)
	{
		QuestSubsystem->AddQuest(FName(CurrentDialogue->QuestID));
	}
}
