// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestOverlay.generated.h"

class FQuest;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class STRAW_API UQuestOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuest(FQuest* Quest);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestTitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestDescText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestQuantityText;
};
