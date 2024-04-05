// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/Triangulator.h"
#include "Math/StrawTriangle.h"
#include "Math/StrawEdge.h"

/// <summary>
/// �־��� Vertices�� ������ ���� ������ ��γ� �ﰢ������ �����ϰ� ��� �ﰢ�� ������ ��ȯ
/// </summary>
/// <param name="DrawingPlaneBoxExtent">�׸��� �׸��� Collision�� Box ����</param>
/// <param name="Vertices">���ʴ�� �̾��� ���� Vertex �迭</param>
/// <returns>�ﰢ���� Vertex </returns>
TArray<StrawTriangle> Triangulator::Triangulate2D(FBox DrawingPlaneBox, TArray<FVector2D> Vertices)
{
	TArray<StrawTriangle> Triangles;

	// 1. Drawing Collision�� ���δ� Box�� �߽ɰ� Extent�� �̿��� ��� Vertex�� ���Խ�ų �� �ִ� Super Triangle ����
	FVector BoxCenter;
	FVector BoxExtent;
	DrawingPlaneBox.GetCenterAndExtents(BoxCenter, BoxExtent);
	BoxExtent *= 3.f;

	FVector2D SuperTriangleP1(BoxCenter.Y - BoxExtent.Y, BoxCenter.Z - BoxExtent.Z);
	FVector2D SuperTriangleP2(BoxCenter.Y, BoxCenter.Z + BoxExtent.Z);
	FVector2D SuperTriangleP3(BoxCenter.Y + BoxExtent.Y, BoxCenter.Z - BoxExtent.Z);
	Triangles.Add(StrawTriangle(SuperTriangleP1, SuperTriangleP2, SuperTriangleP3));

	// 2. ��γ� �ﰢ���� ����
	for (FVector2D Vertex : Vertices)
	{
		Triangles = AddVertex(Triangles, Vertex);
	}

	// 3. Super Triangle�� edge�� �����ϴ� �ﰢ�� ����
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

	// 4. �׸� ���� ����� �ﰢ�� ����

	// 5. �ﰢ���� Vertex�� �ε����� ǥ���ؼ� ��ȯ

	return Triangles;

}

TArray<StrawTriangle> Triangulator::AddVertex(TArray<StrawTriangle> Triangles, FVector2D Vertex)
{
	TArray<StrawEdge> Edges;

	for (int i = Triangles.Num() - 1; i >= 0; i--)
	{
		StrawTriangle Triangle = Triangles[i];

		// �ﰢ���� Vertex�� �����ϰ� �ִٸ� Triangles���� �����ϰ� �ش� Edge�� Vertex�� ���ο� �ﰢ�� ����
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