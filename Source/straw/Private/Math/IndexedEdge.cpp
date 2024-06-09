// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/IndexedEdge.h"

IndexedEdge::IndexedEdge(const TArray<FVector2D>& Vertices, int32 InP1Index, int32 InP2Index)
{
	P1Index = InP1Index;
	P2Index = InP2Index;
	P1 = Vertices[InP1Index];
	P2 = Vertices[InP2Index];
}

IndexedEdge::~IndexedEdge()
{

}

bool IndexedEdge::Equals(IndexedEdge Edge)
{
	return (P1.Equals(Edge.P1) && P2.Equals(Edge.P2)) || (P1.Equals(Edge.P2) && P2.Equals(Edge.P1));
}

bool IndexedEdge::IndexEquals(IndexedEdge Edge)
{
	return (P1Index == Edge.P1Index && P2Index == Edge.P2Index) || (P1Index == Edge.P2Index && P2Index == Edge.P1Index);
}
