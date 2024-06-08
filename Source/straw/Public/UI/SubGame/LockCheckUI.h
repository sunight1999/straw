// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockCheckUI.generated.h"

class URadialSlider;
class UImage;

/**
 * 
 */
UCLASS()
class STRAW_API ULockCheckUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Play();
	void Reset();
	void SetLevel(float CheckBarWidth, float Speed);

	bool Check();

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	URadialSlider* LockCheckSlider;

	UPROPERTY(meta = (BindWidget))
	UImage* LockCheckArrowImage;

	bool bIsPlaying = true;
	float Speed = 40.f;

	float CurrentAngle = 0.f;
};
