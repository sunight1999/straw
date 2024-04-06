// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class StrawTriangle;
class StrawEdge;

/**
 * 
 */
namespace Triangulator
{
	TArray<StrawTriangle> Triangulate2D(FBox DrawingPlaneBox, TArray<FVector2D> Vertices);
	TArray<StrawTriangle> AddVertex(TArray<StrawTriangle> Triangles, FVector2D Vertex);
	TArray<StrawEdge> UniqueEdges(TArray<StrawEdge> Edges);
};
