// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Mover.h"

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner())
	{
		Move(DeltaTime);
		Rotate(DeltaTime);
	}
}

void UMover::BeginPlay()
{
	Super::BeginPlay();
}

void UMover::Move(float DeltaTime)
{
	FVector TargetVector = FMath::VInterpTo(GetOwner()->GetActorLocation(), To, DeltaTime, Velocity);
	GetOwner()->SetActorLocation(TargetVector);
}

void UMover::Rotate(float DeltaTime)
{
	
}


