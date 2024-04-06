// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IndexedTriangle;
class IndexedEdge;

/**
 * 
 */

enum class CCW {
	ClockWise,
	Parallel,
	CounterClockWise
};

namespace Triangulator
{
	TArray<int32> Triangulate2D(FBox DrawingPlaneBox, TArray<FVector> Vertices, TArray<IndexedTriangle>& OutUselessTriangles);

	void AddVertex(TArray<IndexedTriangle>& Triangles, const TArray<FVector2D>& Vertices, const int32 VertexIndex);
	TArray<IndexedEdge> UniqueEdges(TArray<IndexedEdge> Edges);

	bool IsPointInsidePolygon(const TArray<FVector2D>& PolygonVertices, FVector2D Point);
	CCW CounterClockWise(FVector2D A, FVector2D B, FVector2D C);
};
