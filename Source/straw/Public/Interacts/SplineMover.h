// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/Mover.h"
#include "Components/TimelineComponent.h"
#include "SplineMover.generated.h"

class USplineComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class STRAW_API USplineMover : public UMover
{
	GENERATED_BODY()

public:
	USplineMover();

	void Play();
	void SetTarget(AActor* InTarget);

protected:
	void BeginPlay() override;

private:
	void Move(float DeltaTime) override;
	void Rotate(float DeltaTime) override;

	UFUNCTION()
	void MoveAlongSpline(float Value);

	UPROPERTY(EditAnyWhere, Category = "SplineMover")
	USplineComponent* Spline;

	UPROPERTY(EditAnyWhere, Category = "SplineMover")
	UTimelineComponent* MoveTimeline;

	UPROPERTY(EditAnyWhere, Category = "SplineMover")
	UCurveFloat* MoveCurve = nullptr;

	UPROPERTY(EditAnyWhere, Category = "SplineMover")
	float PlayRate = 1.f;

	FOnTimelineFloat MoveCallback;

	AActor* Target = nullptr;

	FVector TargetLocation;
	FRotator TargetRotation;
};
