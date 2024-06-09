// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubGame/LockCheckUI.h"
#include "Components/RadialSlider.h"
#include "Components/Image.h"

void ULockCheckUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsPlaying)
	{
		CurrentAngle += Speed * InDeltaTime;
		if (CurrentAngle > 180)
		{
			CurrentAngle = -180;
		}

		LockCheckArrowImage->SetRenderTransformAngle(CurrentAngle);
	}
}

void ULockCheckUI::Play()
{
	bIsPlaying = true;
}

void ULockCheckUI::Reset()
{
	bIsPlaying = false;
	LockCheckSlider->SetValue(0);
	LockCheckSlider->SetAngularOffset(0);
	LockCheckArrowImage->SetRenderTransformAngle(0);
}

void ULockCheckUI::SetLevel(float CheckBarWidth, float InSpeed)
{
	LockCheckSlider->SetValue(CheckBarWidth);
	LockCheckSlider->SetAngularOffset(FMath::RandRange(0, 360));
	Speed = InSpeed;
}

bool ULockCheckUI::Check()
{
	bIsPlaying = false;

	float CurrentCheckBarWidth = LockCheckSlider->GetValue() * 360;
	float CurrentCheckBarBeginAngle = LockCheckSlider->AngularOffset;
	float CurrentCheckBarEndAngle = CurrentCheckBarBeginAngle + CurrentCheckBarWidth;
	float CurrentArrowAngle = LockCheckArrowImage->GetRenderTransformAngle() + 180;
	
	// 정답 구간이 360도=0도를 포함하는 경우
	if (CurrentCheckBarEndAngle > 360)
	{
		if (CurrentCheckBarBeginAngle <= CurrentArrowAngle || CurrentArrowAngle <= CurrentCheckBarEndAngle - 360)
		{
			return true;
		}
	}
	else if (CurrentCheckBarBeginAngle <= CurrentArrowAngle && CurrentArrowAngle <= CurrentCheckBarEndAngle)
	{
		return true;
	}

	return false;
}
