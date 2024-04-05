// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STRAW_API StrawEdge
{
public:
	StrawEdge(FVector2D _P1, FVector2D _P2);
	~StrawEdge();

	FVector2D GetP1() const { return P1; }
	FVector2D GetP2() const { return P2; }

	bool Equals(StrawEdge Edge);

private:
	FVector2D P1;
	FVector2D P2;
};
