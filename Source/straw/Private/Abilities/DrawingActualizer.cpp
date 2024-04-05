// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DrawingActualizer.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Math/Triangulator.h"
#include "Math/StrawTriangle.h"

// Sets default values
ADrawingActualizer::ADrawingActualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
	ProcMeshComponent->bUseComplexAsSimpleCollision = false;
	//ProcMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void ADrawingActualizer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrawingActualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// ��鿡 �׸� �׸��� 3D�� ��üȭ
/// </summary>
/// <param name="DrawingVertices">������ ���� �� vertex array</param>
/// <param name="DrawingPlaneRotation">�׸��� �׸� ����� Rotation ��</param>
/// <param name="MeshMaterial">��üȭ�� ������Ʈ�� ������ Material</param>
void ADrawingActualizer::Actualize2D(TArray<FVector> DrawingVertices, FBox DrawingPlaneBox, FRotator DrawingPlaneRotation, UMaterialInterface* MeshMaterial)
{
	Material = MeshMaterial;

	// 1. ȸ���Ǿ� �ִ� �׸��� ����(x��)�� �ٶ󺸵��� ȸ������ 2����(y, z)���� ���
	for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		DrawingVertices[i] = DrawingPlaneRotation.UnrotateVector(DrawingVertices[i]);
	}

	// 2. ��γ� �ﰢ���� ����
	TArray<FVector2D> DrawingVertices2D;
	for (FVector Vertex : DrawingVertices)
	{
		DrawingVertices2D.Add(FVector2D(Vertex.Y, Vertex.Z));
	}

	TArray<StrawTriangle> Triangles3D = Triangulator::Triangulate2D(DrawingPlaneBox, DrawingVertices2D);
	/*for (StrawTriangle Triangle : Triangles3D)
	{
		DrawDebugLine(GetWorld(), FVector(0, Triangle.GetP1().X, Triangle.GetP1().Y), FVector(0, Triangle.GetP2().X, Triangle.GetP2().Y), FColor::Red, true);
		DrawDebugLine(GetWorld(), FVector(0, Triangle.GetP2().X, Triangle.GetP2().Y), FVector(0, Triangle.GetP3().X, Triangle.GetP3().Y), FColor::Red, true);
		DrawDebugLine(GetWorld(), FVector(0, Triangle.GetP3().X, Triangle.GetP3().Y), FVector(0, Triangle.GetP1().X, Triangle.GetP1().Y), FColor::Red, true);
	}*/

	// 3. �׸��� ����� �ﰢ�� ����
	// 4. DrawingVertices�� x ���� ���� �޸� Vertex ���� �߰�
	// 5. ���� �ﰢ���� ����
	// 6. ������ object�� ���� ȸ���� ����

	/*
	for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		if (i + 1 == DrawingVertices.Num())
		{
			DrawDebugLine(GetWorld(), DrawingVertices[i-1], DrawingVertices[i], FColor::Red, true);
		}
		else
		{
			DrawDebugLine(GetWorld(), DrawingVertices[i], DrawingVertices[i + 1], FColor::Red, true);
		}
	}
	*/

	/*ProcMeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	if (Material)
	{
		ProcMeshComponent->SetMaterial(0, Material);
	}*/
}