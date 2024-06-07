// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/SplineMover.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"

USplineMover::USplineMover()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeline"));

	MoveTimeline->SetTimelineLength(1.f);
	MoveTimeline->SetLooping(false);

	MoveCallback.BindUFunction(this, TEXT("MoveAlongSpline"));
}

void USplineMover::BeginPlay()
{
	Super::BeginPlay();

	MoveTimeline->SetPlayRate(PlayRate);
	MoveTimeline->AddInterpFloat(MoveCurve, MoveCallback);
}

void USplineMover::Play()
{
	if (!Target)
	{
		Target = GetOwner();
	}
	
	MoveTimeline->Stop();
	MoveTimeline->PlayFromStart();
}

void USplineMover::SetTarget(AActor* InTarget)
{
	Target = InTarget;
}

void USplineMover::Move(float DeltaTime)
{
	if (!MoveTimeline->IsPlaying())
	{
		return;
	}

	FVector Location = FMath::VInterpTo(Target->GetActorLocation(), TargetLocation, DeltaTime, Velocity);
	Target->SetActorLocation(Location);
}

void USplineMover::Rotate(float DeltaTime)
{
	if (!MoveTimeline->IsPlaying())
	{
		return;
	}

	FRotator Rotation = FMath::RInterpTo(Target->GetActorRotation(), TargetRotation, DeltaTime, Velocity);
	Target->SetActorRotation(Rotation);
}

void USplineMover::MoveAlongSpline(float Value)
{
	float LerpValue = FMath::Lerp(0.f, Spline->GetSplineLength(), Value);

	TargetLocation = Spline->GetLocationAtDistanceAlongSpline(LerpValue, ESplineCoordinateSpace::World);
	TargetRotation = Spline->GetRotationAtDistanceAlongSpline(LerpValue, ESplineCoordinateSpace::World);
}
