// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/StrawTriangle.h"

StrawTriangle::StrawTriangle(FVector2D _P1, FVector2D _P2, FVector2D _P3)
{
	P1 = _P1;
	P2 = _P2;
	P3 = _P3;
	
	CalculateCircumcircle();
}

StrawTriangle::~StrawTriangle()
{
	
}

void StrawTriangle::CalculateCircumcircle()
{
	/*
	 * �ﰢ�� p1p2p3�� �ܽ� ���
	 * : ���� p1p2�� ���� p2p3�� ���� �����̵���ϴ� �� ������ �����Ͽ� �ܽ� ���
	 * 
	 * ���� p1p2�� �����̵�м� : y = m1x + b1, y = m'1x + b1	(-> ���� * ���Ⱑ -1�̸� �� ������ ����)
	 * ���� p2p3�� �����̵�м� : y = m2x + b2, y = m'2x + b2
	 * 
	 * ��� ����
	 * 0. �����ﰢ���� ��� ���Ⱑ 0���� ���� ������ ���� �� �����Ƿ� ���� ó�� (�����ﰢ���� ������ ������ �ܽ�)
	 * 1. �� �����̵�м��� ������ ������ b1, b2�� ���� ����
	 * 2. �� �����̵�м��� ������ x�� ���� ���� �� b1, b2 ���� => x ����
	 * 3. (1)���� ���� b1, b2�� (2)���� ���� x�� p1p2�� �����̵�м��� ���� => y ����
	 */

	// �� ���� ��ȭ�� ���
	float DX1 = P2.X - P1.X, DX2 = P3.X - P2.X, DX3 = P1.X - P3.X;
	float DY1 = P2.Y - P1.Y, DY2 = P3.Y - P2.Y, DY3 = P1.Y - P3.Y;

	// 0���� ������ ��츦 ���ϱ� ���� ���⸦ �ٷ� ������� �ʰ� ���� ��
	bool bIsM1Zero = DX1 == 0 || DY1 == 0;
	bool bIsM2Zero = DX2 == 0 || DY2 == 0;
	bool bIsM3Zero = DX3 == 0 || DY3 == 0;

	// 0. �����ﰢ���� ��� ���Ⱑ 0���� ���� ������ ���� �� �����Ƿ� ���� ó��
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

		// ���� ���� p1p2 �Ǵ� p2p3�� ���Ⱑ 0�� ��� ����� ����� �̷����� �����Ƿ� ������ �� ������ ����ϵ��� ����
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

		float M1 = DY1 / DX1;	// p1p2 �����̵�м��� ����
		float M2 = DY2 / DX2;	// p2p3 �����̵�м��� ����
		float MP1 = -(1 / M1);	// M1�� ������ �� -1�� �Ǵ� ��
		float MP2 = -(1 / M2);	// M2�� ������ �� -1�� �Ǵ� ��
		float B1 = MedianOfAB.Y - MP1 * MedianOfAB.X;	// y = m'1x + b1�� b1
		float B2 = MedianOfBC.Y - MP2 * MedianOfBC.X;	// y = m'2x + b2�� b2
		float B2SubB1 = B2 - B1;
		float MP1SubMP2 = MP1 - MP2;
		
		// ��� ������ ���� ���� �����Ŀ� �� ����
		float X = B2SubB1 / MP1SubMP2;	// x = (b2 - b1) / (m'1 - m'2)
		float Y = MP1 * X + B1;	// y = m'1x + b1

		CircumcircleCenter = FVector2D(X, Y);
	}

	SquaredCircumcircleRadius = FVector2D::DistSquared(CircumcircleCenter, P1);
}

/// <summary>
/// �־��� Point�� �ﰢ�� ���ο� �ִ��� ���� ��ȯ
/// </summary>
/// <param name="Point"></param>
/// <returns></returns>
bool StrawTriangle::IsTriangleContainingPoint(FVector2D Point) const
{
	// 1. �ﰢ�� p1p2p3�� ���� ��, ���� p1p2, p2p3, p3p1�� ���� ���� p1P(oint), p2P, p3P�� ����
	// 2. �� �������� ���� ���� ��� ����̰ų� ��� ������ �ﰢ�� ���η� �Ǵ� (�ﰢ���� �� ���� �������� Point�� ��� �������� Ȯ��)
	FVector2D P1P = Point - P1;
	FVector2D P2P = Point - P2;
	FVector2D P3P = Point - P3;

	bool Side1 = FVector2D::CrossProduct(P2 - P1, P1P) >= 0;
	bool Side2 = FVector2D::CrossProduct(P3 - P2, P2P) >= 0;
	bool Side3 = FVector2D::CrossProduct(P1 - P3, P3P) >= 0;

	return Side1 == Side2 == Side3;
}

/// <summary>
/// �־��� Point�� �ﰢ���� ������ ���ο� �ִ��� ���� ��ȯ
/// </summary>
/// <param name="Point"></param>
/// <returns></returns>
bool StrawTriangle::IsCircumcircleContainingPoint(FVector2D Point) const
{
	return FVector2D::DistSquared(Point, CircumcircleCenter) <= SquaredCircumcircleRadius;
}
