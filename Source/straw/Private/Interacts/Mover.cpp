// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Mover.h"

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BeginDelay && BeginDelay > DelayTick)
	{
		DelayTick += DeltaTime;
	}
	else
	{
		if (GetOwner())
		{
			Move(DeltaTime);
			Rotate(DeltaTime);
		}
	}
}

void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
}

void UMover::Move(float DeltaTime)
{
	if (ShouldReturn())
	{
		StartLocation += PlatformVelocity.GetSafeNormal() * MoveDistance;
		GetOwner()->SetActorLocation(StartLocation);
		PlatformVelocity *= -1;
	}
	else
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector TargetVector = FMath::VInterpTo(CurrentLocation, CurrentLocation + PlatformVelocity, DeltaTime, Velocity);

		GetOwner()->SetActorLocation(TargetVector);
	}
}

void UMover::Rotate(float DeltaTime)
{
	
}

bool UMover::ShouldReturn() const
{
	return GetDistanceMoved() > MoveDistance;
}

float UMover::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetOwner()->GetActorLocation());
}


