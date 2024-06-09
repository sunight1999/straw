// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OrnamentOverlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Interacts/Objectives/TraditionalOrnament.h"

bool UOrnamentOverlay::Initialize()
{
	Super::Initialize();

	LeftTopImage->SetVisibility(ESlateVisibility::Hidden);
	LeftBottomImage->SetVisibility(ESlateVisibility::Hidden);
	RightBottomImage->SetVisibility(ESlateVisibility::Hidden);
	RightTopImage->SetVisibility(ESlateVisibility::Hidden);
	CountText->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UOrnamentOverlay::SetBackgroundVisibility(bool InVisibility)
{
	if (InVisibility)
	{
		BackgroundImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOrnamentOverlay::SetOrnamentVisibility(EOrnamentPart OrnamentPart, bool InVisibility)
{
	ESlateVisibility SlateVisibility;
	if (InVisibility)
	{
		SlateVisibility = ESlateVisibility::Visible;
	}
	else
	{
		SlateVisibility = ESlateVisibility::Hidden;
	}

	UImage* PartImage = nullptr;
	switch (OrnamentPart)
	{
	case EOrnamentPart::LeftTop:
		PartImage = LeftTopImage;
		break;
		
	case EOrnamentPart::LeftBottom:
		PartImage = LeftBottomImage;
		break;

	case EOrnamentPart::RightBottom:
		PartImage = RightBottomImage;
		break;

	case EOrnamentPart::RightTop:
		PartImage = RightTopImage;
		break;
	}

	if (PartImage)
	{
		PartImage->SetVisibility(SlateVisibility);
		CountText->SetVisibility(ESlateVisibility::Visible);
		CountText->SetText(FText::FromString(FString::Printf(TEXT("( %d / 4 )"), ++CurrentPieceNum)));
	}
}
