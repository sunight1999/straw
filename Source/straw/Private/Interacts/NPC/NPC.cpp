// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/NPC/NPC.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

#include "Quests/QuestSubsystem.h"
#include "Quests/Quest.h"

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

	// 퀘스트 서브시스템 로드
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
	if (QuestIDs.Num() > 0)
	{
		QuestSubsystem->AddQuest(FName(QuestIDs[0]));
	}
}