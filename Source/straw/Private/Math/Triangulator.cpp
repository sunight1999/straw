// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/Triangulator.h"
#include "Math/StrawTriangle.h"
#include "Math/StrawEdge.h"

/// <summary>
/// 주어진 Vertices로 구성된 면을 가정해 들로네 삼각분할을 진행하고 모든 삼각형 정보를 반환
/// </summary>
/// <param name="DrawingPlaneBoxExtent">그림을 그리는 Collision의 Box 정보</param>
/// <param name="Vertices">차례대로 이어진 선의 Vertex 배열</param>
/// <returns>삼각형의 Vertex </returns>
TArray<StrawTriangle> Triangulator::Triangulate2D(FBox DrawingPlaneBox, TArray<FVector2D> Vertices)
{
	TArray<StrawTriangle> Triangles;

	// 1. Drawing Collision을 감싸는 Box의 중심과 Extent를 이용해 모든 Vertex를 포함시킬 수 있는 Super Triangle 생성
	FVector BoxCenter;
	FVector BoxExtent;
	DrawingPlaneBox.GetCenterAndExtents(BoxCenter, BoxExtent);
	BoxExtent *= 3.f;

	FVector2D SuperTriangleP1(BoxCenter.Y - BoxExtent.Y, BoxCenter.Z - BoxExtent.Z);
	FVector2D SuperTriangleP2(BoxCenter.Y, BoxCenter.Z + BoxExtent.Z);
	FVector2D SuperTriangleP3(BoxCenter.Y + BoxExtent.Y, BoxCenter.Z - BoxExtent.Z);
	Triangles.Add(StrawTriangle(SuperTriangleP1, SuperTriangleP2, SuperTriangleP3));

	// 2. 들로네 삼각분할 수행
	for (FVector2D Vertex : Vertices)
	{
		Triangles = AddVertex(Triangles, Vertex);
	}

	// 3. Super Triangle과 edge를 공유하는 삼각형 제거
	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		StrawTriangle Triangle = Triangles[i];

		if (Triangle.GetP1().Equals(SuperTriangleP1) || Triangle.GetP1().Equals(SuperTriangleP2) || Triangle.GetP1().Equals(SuperTriangleP3) ||
			Triangle.GetP2().Equals(SuperTriangleP1) || Triangle.GetP2().Equals(SuperTriangleP2) || Triangle.GetP2().Equals(SuperTriangleP3) ||
			Triangle.GetP3().Equals(SuperTriangleP1) || Triangle.GetP3().Equals(SuperTriangleP2) || Triangle.GetP3().Equals(SuperTriangleP3))
		{
			Triangles.RemoveAt(i);
		}
	}

	// 4. 그림 밖을 벗어나는 삼각형 제거

	// 5. 삼각형을 Vertex의 인덱스로 표현해서 반환

	return Triangles;

}

TArray<StrawTriangle> Triangulator::AddVertex(TArray<StrawTriangle> Triangles, FVector2D Vertex)
{
	TArray<StrawEdge> Edges;

	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		StrawTriangle Triangle = Triangles[i];

		// 삼각형이 Vertex를 포함하고 있다면 Triangles에서 제거하고 해당 Edge와 Vertex로 새로운 삼각형 생성
		if (Triangle.IsTriangleContainingPoint(Vertex))
		{
			Edges.Add(StrawEdge(Triangle.GetP1(), Triangle.GetP2()));
			Edges.Add(StrawEdge(Triangle.GetP2(), Triangle.GetP3()));
			Edges.Add(StrawEdge(Triangle.GetP3(), Triangle.GetP1()));

			Triangles.RemoveAt(i);
		}
	}

	Edges = UniqueEdges(Edges);

	for (StrawEdge Edge : Edges)
	{
		Triangles.Add(StrawTriangle(Edge.GetP1(), Edge.GetP2(), Vertex));
	}
	
	return Triangles;
}

TArray<StrawEdge> Triangulator::UniqueEdges(TArray<StrawEdge> Edges)
{
	TArray<StrawEdge> UniqueEdges;

	for (int i = 0; i < Edges.Num(); i++)
	{
		bool bIsUnique = true;

		for (int j = 0; j < Edges.Num(); j++)
		{
			if (i != j && Edges[i].Equals(Edges[j]))
			{
				bIsUnique = false;
				break;
			}
		}

		if (bIsUnique)
		{
			UniqueEdges.Add(Edges[i]);
		}
	}

	return UniqueEdges;
}