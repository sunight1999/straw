// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UQuestOverlay;

/**
 * 
 */
UCLASS()
class STRAW_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	UQuestOverlay* GetQuestOverlay() const;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Main)
	TSubclassOf<UQuestOverlay> QuestOverlayClass;

	UQuestOverlay* QuestOverlay;
};
