// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrnamentOverlay.generated.h"

class UImage;
enum class EOrnamentPart : uint8;

/**
 * 
 */
UCLASS()
class STRAW_API UOrnamentOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize() override;

	void SetBackgroundVisibility(bool InVisibility);
	void SetOrnamentVisibility(EOrnamentPart OrnamentPart, bool InVisibility);
	
private:
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftTopImage;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftBottomImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RightBottomImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RightTopImage;
};
