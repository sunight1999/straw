// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Objectives/Pottery.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimSingleNodeInstance.h"

#include "Puzzles/PotteryPuzzle.h"

APottery::APottery()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->AnimationData.bSavedLooping = false;
	SkeletalMesh->AnimationData.bSavedPlaying = false;
}

void APottery::BeginPlay()
{
	Super::BeginPlay();

	// 설정한 Mesh 크기에 맞춰 Interactable UI 위치 조정
	RelocationInteractableWidget(SkeletalMesh);
}

void APottery::Interact()
{
	PotteryPuzzle->NotifyClickedPottery(PotteryID);
}

void APottery::PlayAnimation()
{
	SkeletalMesh->InitAnim(true);
	SkeletalMesh->Play(false);
}

float APottery::GetAnimationLength()
{
	if (AnimationLength <= 0.f)
	{
		float AnimationOriginLength = SkeletalMesh->AnimationData.AnimToPlay->GetPlayLength();
		float AnimationRate = SkeletalMesh->GetPlayRate();

		AnimationLength = AnimationOriginLength / AnimationRate;
	}

	return AnimationLength;
}

void APottery::SetPotteryPuzzle(APotteryPuzzle* InPotteryPuzzle)
{
	PotteryPuzzle = InPotteryPuzzle;
}

void APottery::SetPotteryID(int32 ID)
{
	PotteryID = ID;
}
