// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestSubsystem.h"
#include "Characters/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/MainHUD.h"
#include "HUD/QuestOverlay.h"
#include "Quests/Quest.h"

UQuestSubsystem::UQuestSubsystem()
{
	ConstructorHelpers::FObjectFinder<UDataTable> QuestDataTableFinder(TEXT("/Game/Datas/DT_Quests"));
	if (QuestDataTableFinder.Succeeded())
	{
		QuestDataTable = QuestDataTableFinder.Object;
	}
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubsystem::Deinitialize()
{
	
}

FQuestDetail* UQuestSubsystem::FindQuestDetail(const FName& ID) const
{
	return QuestDataTable->FindRow<FQuestDetail>(ID, FString(""));
}

void UQuestSubsystem::AddQuest(FName QuestID)
{
	if (QuestOverlay == nullptr)
	{
		FetchOverlay();
	}

	FQuestDetail* QuestDetail = FindQuestDetail(QuestID);
	if (!QuestDetail)
	{
		UE_LOG(LogTemp, Warning, TEXT("퀘스트 %s를 찾을 수 없습니다."), QuestID);
		return;
	}

	FQuest* NewQuest = new FQuest(QuestDetail);
	CurrentQuests.Add(NewQuest);

	if (QuestDetail->bIsMain)
	{
		QuestOverlay->SetMainQuest(NewQuest);
	}
	else
	{
		QuestOverlay->AddSubQuest(NewQuest);
	}

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
		QuestOverlay->SetVisibility(ESlateVisibility::Visible);
		QuestOverlay->Update();
	}
	else
	{
		QuestOverlay->SetVisibility(ESlateVisibility::Hidden);
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
