// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class StrawTriangle;

/**
 * 
 */
class STRAW_API Triangulator
{
public:
	Triangulator();
	~Triangulator();

	TArray<int32> CreateTriangulatedMesh(TArray<FVector> Vertices);

private:
	TArray<StrawTriangle> Triangles;
};
