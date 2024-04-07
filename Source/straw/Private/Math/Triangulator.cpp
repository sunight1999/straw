// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/Triangulator.h"
#include "Math/IndexedTriangle.h"
#include "Math/IndexedEdge.h"

/// <summary>
/// 주어진 Vertices로 구성된 면을 가정해 들로네 삼각분할을 진행하고 모든 삼각형의 인덱스 정보를 반환
/// </summary>
/// <param name="DrawingPlaneBoxExtent">그림을 그리는 Collision의 Box 정보</param>
/// <param name="Vertices">차례대로 이어진 선의 Vertex 배열</param>
/// <returns>삼각형의 Vertex </returns>
TArray<int32> Triangulator::Triangulate2D(FBox DrawingPlaneBox, TArray<FVector> Vertices, TArray<IndexedTriangle>& OutUselessTriangles)
{
	// 0. 2차원 벡터로 변환
	TArray<FVector2D> Vertices2D;
	for (const FVector& Vertex : Vertices)
	{
		Vertices2D.Add(FVector2D(Vertex.Y, Vertex.Z));
	}
	
	// 1. Drawing Collision을 감싸는 Box의 중심과 Extent를 이용해 모든 Vertex를 포함시킬 수 있는 Super Triangle 생성
	FVector BoxCenter;
	FVector BoxExtent;
	DrawingPlaneBox.GetCenterAndExtents(BoxCenter, BoxExtent);
	BoxExtent *= 3.f;

	Vertices2D.Add(FVector2D(BoxCenter.Y - BoxExtent.Y, BoxCenter.Z - BoxExtent.Z));
	Vertices2D.Add(FVector2D(BoxCenter.Y, BoxCenter.Z + BoxExtent.Z));
	Vertices2D.Add(FVector2D(BoxCenter.Y + BoxExtent.Y, BoxCenter.Z - BoxExtent.Z));

	int32 SuperTriangleP1Index = Vertices2D.Num() - 3;
	int32 SuperTriangleP2Index = Vertices2D.Num() - 2;
	int32 SuperTriangleP3Index = Vertices2D.Num() - 1;

	TArray<IndexedTriangle> Triangles;
	Triangles.Add(IndexedTriangle(Vertices2D, SuperTriangleP1Index, SuperTriangleP2Index, SuperTriangleP3Index));

	// 2. 들로네 삼각분할 수행
	for (int i = 0; i < Vertices2D.Num(); i++)
	{
		AddVertex(Triangles, Vertices2D, i);
	}

	// 3. Super Triangle과 edge를 공유하는 삼각형 제거
	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		const IndexedTriangle& Triangle = Triangles[i];

		if (Triangle.GetP1Index() == SuperTriangleP1Index || Triangle.GetP1Index() == SuperTriangleP2Index || Triangle.GetP1Index() == SuperTriangleP3Index ||
			Triangle.GetP2Index() == SuperTriangleP1Index || Triangle.GetP2Index() == SuperTriangleP2Index || Triangle.GetP2Index() == SuperTriangleP3Index ||
			Triangle.GetP3Index() == SuperTriangleP1Index || Triangle.GetP3Index() == SuperTriangleP2Index || Triangle.GetP3Index() == SuperTriangleP3Index)
		{
			Triangles.RemoveAt(i);
		}
	}

	// 4. Vertices2D에서 삼각분할을 위해 임시로 추가했던 Super Triangle 정보 제거
	Vertices2D.RemoveAt(Vertices2D.Num() - 1);
	Vertices2D.RemoveAt(Vertices2D.Num() - 1);
	Vertices2D.RemoveAt(Vertices2D.Num() - 1);

	// 5. Vertices가 그리는 면 밖을 벗어나는 삼각형 제거
	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		const IndexedTriangle& Triangle = Triangles[i];
		if (!IsPointInsidePolygon(Vertices2D, (Triangle.GetP1() + Triangle.GetP2()) / 2) ||
			!IsPointInsidePolygon(Vertices2D, (Triangle.GetP2() + Triangle.GetP3()) / 2) ||
			!IsPointInsidePolygon(Vertices2D, (Triangle.GetP3() + Triangle.GetP1()) / 2))
		{
			OutUselessTriangles.Add(Triangle);
			//Triangles.RemoveAt(i);
		}
	}

	// 6. 삼각형을 Vertex의 인덱스로 표현해서 반환
	TArray<int32> TriangleIndices;
	for (const IndexedTriangle& Triangle : Triangles)
	{
		TriangleIndices.Add(Triangle.GetP1Index());
		TriangleIndices.Add(Triangle.GetP2Index());
		TriangleIndices.Add(Triangle.GetP3Index());
	}

	return TriangleIndices;

}

void Triangulator::AddVertex(TArray<IndexedTriangle>& Triangles, const TArray<FVector2D>& Vertices, const int32 VertexIndex)
{
	TArray<IndexedEdge> Edges;

	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		IndexedTriangle Triangle = Triangles[i];

		// 삼각형의 외접원이 Vertex를 포함하고 있다면 Triangles에서 제거하고 해당 Edge와 Vertex로 새로운 삼각형 생성
		if (Triangle.IsCircumcircleContainingPoint(Vertices[VertexIndex]))
		{
			Edges.Add(IndexedEdge(Vertices, Triangle.GetP1Index(), Triangle.GetP2Index()));
			Edges.Add(IndexedEdge(Vertices, Triangle.GetP2Index(), Triangle.GetP3Index()));
			Edges.Add(IndexedEdge(Vertices, Triangle.GetP3Index(), Triangle.GetP1Index()));

			Triangles.RemoveAt(i);
		}
	}

	Edges = UniqueEdges(Edges);

	for (const IndexedEdge& Edge : Edges)
	{
		Triangles.Add(IndexedTriangle(Vertices, Edge.GetP1Index(), Edge.GetP2Index(), VertexIndex));
	}
}

TArray<IndexedEdge> Triangulator::UniqueEdges(TArray<IndexedEdge> Edges)
{
	TArray<IndexedEdge> UniqueEdges;

	for (int i = 0; i < Edges.Num(); i++)
	{
		bool bIsUnique = true;

		for (int j = 0; j < Edges.Num(); j++)
		{
			if (i != j && Edges[i].IndexEquals(Edges[j]))
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

/// <summary>
/// Point가 다각형 내부에 있는지 여부 반환
/// </summary>
/// <param name="PolygonVertices">다각형을 이루는 Vertex 배열</param>
/// <param name="Point">다각형 내부에 있는지 확인할 Point</param>
/// <returns>다각형 내부 존재 여부</returns>
bool Triangulator::IsPointInsidePolygon(const TArray<FVector2D>& PolygonVertices, FVector2D Point)
{
	// Point에서 오른쪽으로 가상의 반직선을 긋고, 해당 반직선이 다각형의 변과 몇 변 교차하는지 확인 -> 홀수면 내부
	int32 CrossedCount = 0;
	for (int i = 0; i < PolygonVertices.Num(); i++)
	{
		int32 NextPolygonVertexIndex = i + 1;
		if (i + 1 == PolygonVertices.Num())
		{
			NextPolygonVertexIndex = 0;
		}

		FVector2D P1 = PolygonVertices[i];
		FVector2D P2 = PolygonVertices[NextPolygonVertexIndex];

		// 방향을 계산할 때 일관성을 위해 항상 P1이 P2 위에 있도록 설정
		if (P1.Y < P2.Y)
		{
			P1 = PolygonVertices[NextPolygonVertexIndex];
			P2 = PolygonVertices[i];
		}

		float MinX = std::min<float>(P1.X, P2.X);
		float MinY = std::min<float>(P1.Y, P2.Y);
		float MaxX = std::max<float>(P1.X, P2.X);
		float MaxY = std::max<float>(P1.Y, P2.Y);

		// Point의 반직선과 선분 P1P2가 평행한 경우 처리
		CCW Direction = CounterClockWise(P1, Point, P2);
		if (Direction == CCW::Parallel)
		{
			if (MinX <= Point.X && Point.X <= MaxX && MinY <= Point.Y && Point.Y <= MaxY)
			{
				return true;
			}
		}

		if (MaxX < Point.X) continue;	// 다각형의 변이 Point 좌측에 있으면 교차 X
		if (P1.Y <= Point.Y) continue;	// 다각형의 변이 Point 아래에 있으면 교차 X
		if (P2.Y > Point.Y) continue;	// 다각형의 변이 Point 위에 있으면 교차 X
		
		if (Direction == CCW::CounterClockWise) CrossedCount++;
	}

	return CrossedCount % 2 != 0;
}

/// <summary>
/// 점 A, B, C를 순서대로 봤을 때 방향이 시계 방향인지 반시계 방향인지 판별
/// </summary>
/// <param name="A">점 A</param>
/// <param name="B">점 B</param>
/// <param name="C">점 C</param>
/// <returns>방향</returns>
CCW Triangulator::CounterClockWise(FVector2D A, FVector2D B, FVector2D C)
{
	FVector2D AB = B - A;
	FVector2D BC = C - B;

	// 두 벡터의 외적으로 방향 판별
	float Value = AB.X * BC.Y - AB.Y * BC.X;

	if (Value < 0) return CCW::ClockWise;
	if (Value == 0) return CCW::Parallel;
	return CCW::CounterClockWise;
}
