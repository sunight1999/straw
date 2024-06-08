// Fill out your copyright notice in the Description page of Project Settings.


#include "SubGame/LockCheckSubsystem.h"

#include "HUD/MainHUD.h"
#include "UI/SubGame/LockCheckUI.h"

void ULockCheckSubsystem::FetchUI()
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

	LockCheckUI = MainHUD->GetLockCheckUI();
}

void ULockCheckSubsystem::StartLockCheck()
{
	if (!LockCheckUI)
	{
		FetchUI();
	}

	bIsPlaying = true;
	LockCheckUI->SetVisibility(ESlateVisibility::Visible);

	CurrentLevel = 0;

	Next();
}

void ULockCheckSubsystem::Check()
{
	bool bCheckResult = LockCheckUI->Check();
	if (bCheckResult)
	{
		Next();
	}
	else
	{
		End(false);
	}
}

void ULockCheckSubsystem::Next()
{
	if (CurrentLevel++ < MaxLevel)
	{
		float CheckBarWidth = StartCheckBarWidth + CheckBarWidthStep * CurrentLevel;
		float Speed = StartSpeed + SpeedStep * CurrentLevel;
		LockCheckUI->SetLevel(CheckBarWidth, Speed);
		LockCheckUI->Play();
	}
	else
	{
		End(true);
	}
}

void ULockCheckSubsystem::End(bool bResult)
{
	bIsPlaying = false;
	LockCheckUI->SetVisibility(ESlateVisibility::Hidden);
	LockCheckEndDelegate.Execute(bResult);
}
