// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/IndicatorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "UI/IndicatorUI.h"

UIndicatorComponent::UIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorWidget)
	{
		IndicatorWidget->AddToViewport();
	}
}


// Called every frame
void UIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PlayerController == nullptr)
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
	}

	if (Owner == nullptr)
	{
		Owner = GetOwner();
	}

	// indicator UI가 viewport 밖으로 벗어나지 않도록 위치 설정
	if (PlayerController && Owner)
	{
		FVector2D ScreenLocation;
		PlayerController->ProjectWorldLocationToScreen(Owner->GetActorLocation(), ScreenLocation);
		ScreenLocation.X += IndicatorOffset / 2;
		ScreenLocation.Y += IndicatorOffset / 2;

		int32 SizeX, SizeY;
		PlayerController->GetViewportSize(SizeX, SizeY);
		ScreenLocation.X = FMath::Clamp(ScreenLocation.X, 0, SizeX + IndicatorOffset);
		ScreenLocation.Y = FMath::Clamp(ScreenLocation.Y, 0, SizeY + IndicatorOffset);

		IndicatorWidget->SetPositionInViewport(ScreenLocation);
	}
}

