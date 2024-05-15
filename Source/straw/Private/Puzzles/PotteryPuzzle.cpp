// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/PotteryPuzzle.h"
#include "Kismet/GameplayStatics.h"

#include "Interacts/Objectives/Pottery.h"

void APotteryPuzzle::BeginPlay()
{
	Super::BeginPlay();

	// 맵에 배치되어 있는 Pottery 오브젝트 검색
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APottery::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		if (APottery* Pottery = Cast<APottery>(Actor))
		{
			Pottery->SetPotteryPuzzle(this);
			Potteries.Add(Pottery);
		}
	}

	if (Potteries.Num() == 0)
	{
		return;
	}

	for (int i = 0; i < Potteries.Num(); i++)
	{
		Potteries[i]->SetPotteryID(i);
	}

	AnimationLength = Potteries[0]->GetAnimationLength();
}

void APotteryPuzzle::BeginPuzzle()
{
	BeginLevel();
}

void APotteryPuzzle::EndPuzzle()
{
	// 플레이어가 맞힐 수 없게 마구잡이로 출력하다가 고양이가 어지러워서 포기하는 씬 재생
	UE_LOG(LogTemp, Display, TEXT("CORRECT PUZZLE GOOD!"));
}

void APotteryPuzzle::BeginLevel(bool bIsCorrect)
{
	bIsLevelReady = false;
	CurrentAnswerIndex = 0;
	CurrentPlayIndex = 0;
	Answers.Empty();

	// 정답을 맞혔을 경우 다음 단계로 수치 조정
	if (bIsCorrect)
	{
		++CurrentLevel;
	}
	
	if (CurrentLevel == MaxLevel)
	{
		EndPuzzle();
		return;
	}

	// 정답 생성
	CurrentAnswerNum = StartAnswerNum + StepAnswerNum * CurrentLevel;
	for (int i = 0; i < CurrentAnswerNum; i++)
	{
		int32 Answer = FMath::RandRange(0, Potteries.Num() - 1);
		Answers.Add(Answer);
	}

	// 정답 순서대로 도자기 애니메이션 재생
	GetWorldTimerManager().SetTimer(PotteryAnimTimer, this, &APotteryPuzzle::PlayCurrentPotteryAnim, AnimationLength + AnimationInterval, true, 0.f);
}

void APotteryPuzzle::PlayCurrentPotteryAnim()
{
	if (CurrentPlayIndex == CurrentAnswerNum)
	{
		GetWorldTimerManager().ClearTimer(PotteryAnimTimer);
		bIsLevelReady = true;

		return;
	}

	Potteries[Answers[CurrentPlayIndex++]]->PlayAnimation();
}

void APotteryPuzzle::NotifyClickedPottery(int32 PotteryID)
{
	if (!bIsLevelReady)
	{
		return;
	}

	if (Answers[CurrentAnswerIndex++] != PotteryID)
	{
		WrongAnswer();
		return;
	}

	if (CurrentAnswerIndex == CurrentAnswerNum)
	{
		CorrectAnswer();
	}
}

void APotteryPuzzle::CorrectAnswer()
{
	// 정답을 모두 맞혔을 때 나올 애니메이션, 효과음, 스크립트 등 재생
	UE_LOG(LogTemp, Display, TEXT("CORRECT LEVEL %d"), CurrentLevel);

	BeginLevel(true);
}

void APotteryPuzzle::WrongAnswer()
{
	// 틀렸을 때 나올 애니메이션, 효과음, 스크립트 등 재생
	UE_LOG(LogTemp, Display, TEXT("WRONG! THE ANSWER IS %d"), Answers[CurrentAnswerIndex - 1]);

	BeginLevel();
}