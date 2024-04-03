// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STRAW_API StrawTriangle
{
public:
	StrawTriangle(FVector2D _P1, FVector2D _P2, FVector2D _P3);
	~StrawTriangle();

	FVector2D GetCircumcircleCenter() const { return CircumcircleCenter; }
	float GetSquaredCircumcircleRadius() const { return SquaredCircumcircleRadius; }

	void CalculateCircumcircle();
	bool IsTriangleContainingPoint(FVector2D P) const;
	bool IsCircumcircleContainingPoint(FVector2D P) const;

private:
	FVector2D P1;
	FVector2D P2;
	FVector2D P3;

	FVector2D CircumcircleCenter;
	float SquaredCircumcircleRadius;

};
