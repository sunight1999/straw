// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/NPC/NPC.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/QuestSubsystem.h"
#include "Quests/Quest.h"

ANPC::ANPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ANPC::Interact()
{
	Talk();
}

void ANPC::Talk()
{
	// 1. 다이얼로그 존재 여부 확인 후 출력 코드 필요

	// 2. 해당 다이얼로그에서 연관 퀘스트 아이디 받아오기

	// 현재 다이얼로그 시스템이 없으므로 일단 퀘스트를 바로 부여
	QuestSubsystem->AddQuest(QuestsMap[QuestIDs[0]]);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		return;
	}

	QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>();
	if (!QuestSubsystem)
	{
		return;
	}

	for (FString QuestID : QuestIDs)
	{
		FQuestDetail* QuestDetail = QuestSubsystem->FindQuestDetail(FName(QuestID));
		if (!QuestDetail)
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot find Quest %s from quest data table."), *QuestID);
			break;
		}

		QuestsMap.Add(QuestID, QuestDetail);
	}
}