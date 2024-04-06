// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class STRAW_API IndexedEdge
{
public:
	IndexedEdge(const TArray<FVector2D>& Vertices, int32 InP1Index, int32 InP2Index);
	~IndexedEdge();

	FVector2D GetP1() const { return P1; }
	FVector2D GetP2() const { return P2; }
	int32 GetP1Index() const { return P1Index; }
	int32 GetP2Index() const { return P2Index; }

	bool Equals(IndexedEdge Edge);
	bool IndexEquals(IndexedEdge Edge);

private:
	FVector2D P1, P2;
	int32 P1Index, P2Index, P3Index;
};
