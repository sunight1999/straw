// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UQuestOverlay;
class UOrnamentOverlay;

/**
 * 
 */
UCLASS()
class STRAW_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	UQuestOverlay* GetQuestOverlay() const;
	UOrnamentOverlay* GetOrnamentOverlay() const;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Main)
	TSubclassOf<UQuestOverlay> QuestOverlayClass;

	UPROPERTY(EditDefaultsOnly, Category = Main)
	TSubclassOf<UOrnamentOverlay> OrnamentOverlayClass;

	UQuestOverlay* QuestOverlay;
	UOrnamentOverlay* OrnamentOverlay;
};
