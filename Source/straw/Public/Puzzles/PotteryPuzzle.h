// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puzzles/Puzzle.h"
#include "PotteryPuzzle.generated.h"

class APottery;

/**
 * 
 */
UCLASS()
class STRAW_API APotteryPuzzle : public APuzzle
{
	GENERATED_BODY()
	
public:
	void BeginPuzzle() override;
	void EndPuzzle() override;

	void BeginLevel(bool bIsCorrect = false);
	void PlayCurrentPotteryAnim();
	
	void NotifyClickedPottery(int32 PotteryID);
	void CorrectAnswer();
	void WrongAnswer();

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "PotteryPuzzle")
	int32 MaxLevel = 3;
	
	UPROPERTY(EditAnywhere, Category = "PotteryPuzzle")
	int32 StartAnswerNum = 3;

	UPROPERTY(EditAnywhere, Category = "PotteryPuzzle")
	int32 StepAnswerNum = 2;

	UPROPERTY(EditAnywhere, Category = "PotteryPuzzle")
	float AnimationInterval = .2f;

	bool bIsLevelReady = false;
	int32 CurrentLevel = 0;
	int32 CurrentAnswerNum = 0;
	int32 CurrentAnswerIndex = 0;

	FTimerHandle PotteryAnimTimer;
	float AnimationLength = 0.f;
	int32 CurrentPlayIndex = 0;

	TArray<APottery*> Potteries;
	TArray<int32> Answers;
};
