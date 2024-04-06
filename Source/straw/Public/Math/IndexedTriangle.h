// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class STRAW_API IndexedTriangle
{
public:
	IndexedTriangle(const TArray<FVector2D>& Vertices, int32 InP1Index, int32 InP2Index, int32 InP3Index, bool bCalculateCircumcircle = true);
	IndexedTriangle(FVector2D InP1, FVector2D InP2, FVector2D InP3, bool bCalculateCircumcircle = true);
	~IndexedTriangle();

	FVector2D GetCircumcircleCenter() const { return CircumcircleCenter; }
	float GetSquaredCircumcircleRadius() const { return SquaredCircumcircleRadius; }
	FVector2D GetP1() const { return P1; }
	FVector2D GetP2() const { return P2; }
	FVector2D GetP3() const { return P3; }
	int32 GetP1Index() const { return P1Index; }
	int32 GetP2Index() const { return P2Index; }
	int32 GetP3Index() const { return P3Index; }

	float GetArea() const;
	bool IsIndexed() const { return bIsIndexed; }
	bool IsTriangleContainingPoint(FVector2D Point) const;
	bool IsCircumcircleContainingPoint(FVector2D Point) const;

private:
	bool bIsIndexed;

	FVector2D P1, P2, P3;
	int32 P1Index, P2Index, P3Index;

	FVector2D CircumcircleCenter;
	float SquaredCircumcircleRadius;

	void CalculateCircumcircle();
};
