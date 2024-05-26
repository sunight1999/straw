// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestOverlay.generated.h"

class FQuest;
class UImage;
class UTextBlock;
class UVerticalBox;
class USubQuestUI;

/**
 * 
 */
UCLASS()
class STRAW_API UQuestOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMainQuest(FQuest* Quest);
	void AddSubQuest(FQuest* SubQuest);
	void RemoveSubQuest(FQuest* SubQuest);

	void Update();

private:
	/* 메인 퀘스트 */
	UPROPERTY(meta = (BindWidget))
	UImage* MainSymbolImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainQuestTitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainQuestDescText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainQuestQuantityText;

	/* 서브 퀘스트 */
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SubQuestVBox;

	FQuest* MainQuest;
};
