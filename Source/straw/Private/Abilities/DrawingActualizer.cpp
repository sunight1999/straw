// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DrawingActualizer.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Math/Triangulator.h"
#include "Math/IndexedTriangle.h"

// Sets default values
ADrawingActualizer::ADrawingActualizer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
	ProcMeshComponent->bUseComplexAsSimpleCollision = false;
	ProcMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMeshComponent->SetSimulatePhysics(true);
	ProcMeshComponent->SetMassScale(NAME_None, 1000.f);

	UVs.Add(FVector2D(0, 0));
	UVs.Add(FVector2D(0, 1));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(1, 1));
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
/// <param name="DrawingPlaneBox">그림을 그린 평면을 감싸는 Box</param>
/// <param name="DrawingPlaneRotation">그림을 그린 평면의 Rotation 값</param>
/// <param name="ActualizationThickness">실체화된 오브젝트의 두께</param>
/// <param name="MeshMaterial">실체화된 오브젝트에 적용할 Material</param>
/// <return>생성된 오브젝트의 중심 좌표</return>
FVector ADrawingActualizer::Actualize2D(TArray<FVector> DrawingVertices, FBox DrawingPlaneBox, FRotator DrawingPlaneRotation, float ActualizationThickness, UMaterialInterface* MeshMaterial)
{
	Material = MeshMaterial;

	// 1. 회전되어 있는 그림을 정면(x축)을 바라보도록 회전시켜 2차원(y, z)으로 축소
	for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		Vertices.Add(DrawingPlaneRotation.UnrotateVector(DrawingVertices[i]));
	}

	// 각 포인트 사이의 중점에 포인트를 추가
	/*for (int i = 0; i < DrawingVertices.Num(); i++)
	{
		if (i + 1 < DrawingVertices.Num())
		{
			Vertices.Insert((Vertices[i * 2] + Vertices[i * 2 + 1]) / 2, i * 2 + 1);
		}
	}*/

	//Vertices.Add(Vertices[0] + Vertices[Vertices.Num() - 1] / 2);

	/* 테스트용
	Vertices.Empty();
	Vertices.Add(FVector(0, -50, -50));
	Vertices.Add(FVector(0, -50, 50));
	Vertices.Add(FVector(0, 50, 50));
	Vertices.Add(FVector(0, 30, 25));
	//Vertices.Add(FVector(0, 40, -12.5));
	Vertices.Add(FVector(0, 50, -50));
	Vertices.Add(FVector(0, 30, -25));
	*/

	// 2. 들로네 삼각분할 수행
	TArray<IndexedTriangle> UselessTriangles;
	Triangles = Triangulator::Triangulate2D(DrawingPlaneBox, Vertices, UselessTriangles);

	/*for (FVector Point : Vertices)
	{
		UE_LOG(LogTemp, Display, TEXT("%f %f %f"), Point.X, Point.Y, Point.Z);
		DrawDebugPoint(GetWorld(), Point, 3.f, FColor::Blue, true);
	}

	// 삼각분할 라인 테스트용
	for (int i = 0; i < Triangles.Num(); i += 3)
	{
		DrawDebugLine(GetWorld(), Vertices[Triangles[i]], Vertices[Triangles[i + 1]], FColor::Red, true);
		DrawDebugLine(GetWorld(), Vertices[Triangles[i + 1]], Vertices[Triangles[i + 2]], FColor::Red, true);
		DrawDebugLine(GetWorld(), Vertices[Triangles[i + 2]], Vertices[Triangles[i]], FColor::Red, true);
	}
	*/

	/*
	for (int i = 0; i < UselessTriangles.Num(); i++)
	{
		FVector2D A = (UselessTriangles[i].GetP1() + UselessTriangles[i].GetP2()) / 2;
		FVector2D B = (UselessTriangles[i].GetP2() + UselessTriangles[i].GetP3()) / 2;
		FVector2D C = (UselessTriangles[i].GetP3() + UselessTriangles[i].GetP1()) / 2;
		FVector AA(0, A.X, A.Y);
		FVector BB(0, B.X, B.Y);
		FVector CC(0, C.X, C.Y);

		DrawDebugPoint(GetWorld(), AA, 2, FColor::Green, true);
		DrawDebugPoint(GetWorld(), BB, 2, FColor::Green, true);
		DrawDebugPoint(GetWorld(), CC, 2, FColor::Green, true);
		DrawDebugLine(GetWorld(), FVector(0, UselessTriangles[i].GetP1().X, UselessTriangles[i].GetP1().Y), FVector(0, UselessTriangles[i].GetP2().X, UselessTriangles[i].GetP2().Y), FColor::Blue, true);
		DrawDebugLine(GetWorld(), FVector(0, UselessTriangles[i].GetP2().X, UselessTriangles[i].GetP2().Y), FVector(0, UselessTriangles[i].GetP3().X, UselessTriangles[i].GetP3().Y), FColor::Blue, true);
		DrawDebugLine(GetWorld(), FVector(0, UselessTriangles[i].GetP3().X, UselessTriangles[i].GetP3().Y), FVector(0, UselessTriangles[i].GetP1().X, UselessTriangles[i].GetP1().Y), FColor::Blue, true);
	}
	*/

	// 3. 들로네 삼각분할로 얻은 면 정보에 두께 값(x축)만 더해 뒷면 mesh 정보 추가
	int32 OriginVerticesNum = Vertices.Num();
	for (int i = 0; i < OriginVerticesNum; i++)
	{
		Vertices.Add(FVector(Vertices[i].X + ActualizationThickness, Vertices[i].Y, Vertices[i].Z));
	}

	int32 OriginTriangleIndicesNum = Triangles.Num();
	for (int i = 0; i < OriginTriangleIndicesNum; i++)
	{
		Triangles.Add(Triangles[i] + OriginVerticesNum);
	}
	
	// 4. 옆면 삼각분할 수행
	for (int i = 0; i < OriginVerticesNum - 1; i++)
	{
		Triangles.Add(i + 1);
		Triangles.Add(i + OriginVerticesNum);
		Triangles.Add(i);

		Triangles.Add(i + OriginVerticesNum + 1);
		Triangles.Add(i + OriginVerticesNum);
		Triangles.Add(i + 1);
	}

	Triangles.Add(0);
	Triangles.Add(OriginVerticesNum * 2 - 1);
	Triangles.Add(OriginVerticesNum - 1);

	Triangles.Add(OriginVerticesNum);
	Triangles.Add(OriginVerticesNum * 2 - 1);
	Triangles.Add(0);

	ProcMeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	ProcMeshComponent->AddCollisionConvexMesh(Vertices);
	ProcMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	if (Material)
	{
		ProcMeshComponent->SetMaterial(0, Material);
	}

	return ProcMeshComponent->GetCenterOfMass();
}