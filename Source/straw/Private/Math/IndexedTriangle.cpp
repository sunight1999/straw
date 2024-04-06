// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/IndexedTriangle.h"

IndexedTriangle::IndexedTriangle(const TArray<FVector2D>& Vertices, int32 InP1Index, int32 InP2Index, int32 InP3Index, bool bCalculateCircumcircle) :
	P1Index(InP1Index), P2Index(InP2Index), P3Index(InP3Index), P1(Vertices[InP1Index]), P2(Vertices[InP2Index]), P3(Vertices[InP3Index])
{
	if (bCalculateCircumcircle)
	{
		CalculateCircumcircle();
	}

	bIsIndexed = true;
}

IndexedTriangle::IndexedTriangle(FVector2D InP1, FVector2D InP2, FVector2D InP3, bool bCalculateCircumcircle) :
	P1(InP1), P2(InP2), P3(InP3)
{
	if (bCalculateCircumcircle)
	{
		CalculateCircumcircle();
	}

	bIsIndexed = false;
}

IndexedTriangle::~IndexedTriangle()
{

}

void IndexedTriangle::CalculateCircumcircle()
{
	/*
	 * 삼각형 p1p2p3의 외심 계산
	 * : 선분 p1p2와 선분 p2p3을 각각 수직이등분하는 두 직선을 연립하여 외심 계산
	 *
	 * 선분 p1p2와 수직이등분선 : y = m1x + b1, y = m'1x + b1	(-> 기울기 * 기울기가 -1이면 두 선분은 직교)
	 * 선분 p2p3와 수직이등분선 : y = m2x + b2, y = m'2x + b2
	 *
	 * 계산 절차
	 * 0. 직각삼각형의 경우 기울기가 0으로 나와 오류가 생길 수 있으므로 따로 처리 (직각삼각형은 빗변의 중점이 외심)
	 * 1. 각 수직이등분선에 중점을 대입해 b1, b2에 대해 정리
	 * 2. 두 수직이등분선을 연립해 x에 대해 정리 후 b1, b2 대입 => x 도출
	 * 3. (1)에서 나온 b1, b2와 (2)에서 나온 x를 p1p2의 수직이등분선에 대입 => y 도출
	 */

	 // 각 변의 변화량 계산
	float DX1 = P2.X - P1.X, DX2 = P3.X - P2.X, DX3 = P1.X - P3.X;
	float DY1 = P2.Y - P1.Y, DY2 = P3.Y - P2.Y, DY3 = P1.Y - P3.Y;

	// 0으로 나누는 경우를 피하기 위해 기울기를 바로 계산하지 않고 각각 비교
	bool bIsM1Zero = DX1 == 0 || DY1 == 0;
	bool bIsM2Zero = DX2 == 0 || DY2 == 0;
	bool bIsM3Zero = DX3 == 0 || DY3 == 0;

	// 0. 직각삼각형의 경우 기울기가 0으로 나와 오류가 생길 수 있으므로 따로 처리
	int8 ZeroSlopeCount = bIsM1Zero + bIsM2Zero + bIsM3Zero;
	if (ZeroSlopeCount >= 2)
	{
		if (!bIsM1Zero)
		{
			CircumcircleCenter = (P1 + P2) / 2;
		}
		else if (!bIsM2Zero)
		{
			CircumcircleCenter = (P2 + P3) / 2;
		}
		else
		{
			CircumcircleCenter = (P3 + P1) / 2;
		}
	}
	else
	{
		FVector2D A = P1, B = P2, C = P3;
		bool bIsSlopeDirty = true;

		// 만약 선분 p1p2 또는 p2p3의 기울기가 0일 경우 계산이 제대로 이뤄지지 않으므로 나머지 두 선분을 사용하도록 설정
		if (bIsM1Zero)
		{
			A = P2;
			B = P3;
			C = P1;
		}
		else if (bIsM2Zero)
		{
			A = P3;
			B = P1;
			C = P2;
		}
		else
		{
			bIsSlopeDirty = false;
		}

		if (bIsSlopeDirty)
		{
			DX1 = B.X - A.X, DX2 = C.X - B.X;
			DY1 = B.Y - A.Y, DY2 = C.Y - B.Y;
		}

		FVector2D MedianOfAB = (A + B) / 2;
		FVector2D MedianOfBC = (B + C) / 2;

		float M1 = DY1 / DX1;	// p1p2 수직이등분선의 기울기
		float M2 = DY2 / DX2;	// p2p3 수직이등분선의 기울기
		float MP1 = -(1 / M1);	// M1과 곱했을 때 -1이 되는 수
		float MP2 = -(1 / M2);	// M2와 곱했을 때 -1이 되는 수
		float B1 = MedianOfAB.Y - MP1 * MedianOfAB.X;	// y = m'1x + b1의 b1
		float B2 = MedianOfBC.Y - MP2 * MedianOfBC.X;	// y = m'2x + b2의 b2
		float B2SubB1 = B2 - B1;
		float MP1SubMP2 = MP1 - MP2;

		// 계산 절차를 거쳐 나온 방정식에 값 대입
		float X = B2SubB1 / MP1SubMP2;	// x = (b2 - b1) / (m'1 - m'2)
		float Y = MP1 * X + B1;	// y = m'1x + b1

		CircumcircleCenter = FVector2D(X, Y);
	}

	SquaredCircumcircleRadius = FVector2D::DistSquared(CircumcircleCenter, P1);
}

float IndexedTriangle::GetArea() const
{
	return abs((P1.X * P2.Y + P2.X * P3.Y + P3.X * P1.Y) - (P1.X * P3.Y + P3.X * P2.Y + P2.X * P1.Y)) / 2;
}

/// <summary>
/// 주어진 Point가 삼각형 내부에 있는지 여부 반환
/// </summary>
/// <param name="Point"></param>
/// <returns></returns>
bool IndexedTriangle::IsTriangleContainingPoint(FVector2D Point) const
{
	//삼각형 p1p2p3이 있을 때, 삼각형 p1p2p, p1p3p, p2p3p의 넓이의 합이 삼각형 p1p2p3의 넓이와 같은지 확인해 내부 여부 판별
	IndexedTriangle Triangle1(P1, P2, Point, false);
	IndexedTriangle Triangle2(P1, P3, Point, false);
	IndexedTriangle Triangle3(P2, P3, Point, false);

	return abs(Triangle1.GetArea() + Triangle2.GetArea() + Triangle3.GetArea() - GetArea()) < DBL_EPSILON;
}

/// <summary>
/// 주어진 Point가 삼각형의 외접원 내부에 있는지 여부 반환
/// </summary>
/// <param name="Point"></param>
/// <returns></returns>
bool IndexedTriangle::IsCircumcircleContainingPoint(FVector2D Point) const
{
	return FVector2D::DistSquared(Point, CircumcircleCenter) <= SquaredCircumcircleRadius;
}
