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
/// 평면에 그린 그림을 3D로 실체화
/// </summary>
/// <param name="DrawingVertices">끊김이 없는 선 vertex array</param>
/// <param name="DrawingPlaneRotation">그림을 그린 평면의 Rotation 값</param>
/// <param name="MeshMaterial">실체화된 오브젝트에 적용할 Material</param>
void ADrawingActualizer::Actualize2D(TArray<FVector> DrawingVertices, FBox DrawingPlaneBox, FRotator DrawingPlaneRotation, UMaterialInterface* MeshMaterial)
{
	Material = MeshMaterial;

	// 1. 회전되어 있는 그림을 정면(x축)을 바라보도록 회전시켜 2차원(y, z)으로 축소
	for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		DrawingVertices[i] = DrawingPlaneRotation.UnrotateVector(DrawingVertices[i]);
	}

	// 2. 들로네 삼각분할 수행
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

	// 3. 그림을 벗어나는 삼각형 제거
	// 4. DrawingVertices에 x 값을 더해 뒷면 Vertex 정보 추가
	// 5. 옆면 삼각분할 수행
	// 6. 생성된 object에 원래 회전값 적용

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