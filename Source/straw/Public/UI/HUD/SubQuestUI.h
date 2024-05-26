// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubQuestUI.generated.h"

class UTextBlock;
class UImage;
class FQuest;

/**
 * 
 */
UCLASS()
class STRAW_API USubQuestUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuest(FQuest* Quest);
	FQuest* GetSubQuest() const { return SubQuest; }

	void Update();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubQuestDescText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubQuestQuantityText;

	FQuest* SubQuest;
};
