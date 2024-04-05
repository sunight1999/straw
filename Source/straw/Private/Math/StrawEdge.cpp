// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/StrawEdge.h"

StrawEdge::StrawEdge(FVector2D _P1, FVector2D _P2) :
	P1(_P1), P2(_P2)
{
	
}

StrawEdge::~StrawEdge()
{

}

bool StrawEdge::Equals(StrawEdge Edge)
{
	return (P1.Equals(Edge.P1) && P2.Equals(Edge.P2)) || (P1.Equals(Edge.P2) && P2.Equals(Edge.P1));
}

