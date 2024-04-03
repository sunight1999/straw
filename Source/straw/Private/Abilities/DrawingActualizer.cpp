// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DrawingActualizer.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

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
/// <param name="DrawingVertices">�� vertex array, ��� �̾��� �־�� ��</param>
/// <param name="DrawingPlaneRotation">�׸��� �׸� ����� Rotation ��</param>
/// <param name="MeshMaterial">��üȭ�� ������Ʈ�� ������ Material</param>
void ADrawingActualizer::Actualize2DDrwaing(TArray<FVector> DrawingVertices, FRotator DrawingPlaneRotation, UMaterialInterface* MeshMaterial)
{
	Material = MeshMaterial;

	/*Vertices.Add(FVector(-50, 0, 50));
	Vertices.Add(FVector(-50, 0, -50));
	Vertices.Add(FVector(50, 0, 50));
	Vertices.Add(FVector(50, 0, -50));

	UVs.Add(FVector2D(0, 0));
	UVs.Add(FVector2D(0, 1));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(1, 1));

	//Triangle1
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	//Triangle2
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(3);
	*/

	// 1. ȸ���Ǿ� �ִ� �׸��� ����(x��)�� �ٶ󺸵��� ȸ������ 2����(y, z)���� ���
	for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		DrawingVertices[i] = DrawingPlaneRotation.UnrotateVector(DrawingVertices[i]);
	}

	// 2. ��γ� �ﰢ���� ����
	
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