// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Objectives/TraditionalOrnament.h"
#include "Components/WidgetComponent.h"

ATraditionalOrnament::ATraditionalOrnament()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InteractableWidget->SetVisibility(false);
}

EOrnamentPart ATraditionalOrnament::GetOrnamentPart() const
{
	return OrnamentPart;
}

void ATraditionalOrnament::Root()
{
	Destroy();
}
