// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LineDrawingComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

ULineDrawingComponent::ULineDrawingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ULineDrawingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULineDrawingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bDrawing)
	{
		DrawingTick += DeltaTime;
	}
}

/// <summary>
/// 이전에 그렸던 정보를 모두 초기화
/// </summary>
void ULineDrawingComponent::StartDrawing()
{
	SplineComponent->ClearSplinePoints();

	for (int i = 0; i < SplineMeshes.Num(); i++)
	{
		USplineMeshComponent* SplineMesh = SplineMeshes[i];
		if (SplineMesh)
		{
			SplineMeshes[i]->DestroyComponent();
		}
	}
	SplineMeshes.Empty();

	DrawingTick = DrawingInterval + 1;	// 시작 지점을 지연 없이 바로 그릴 수 있도록 DrawingInterval에 1을 더한 값으로 초기화
	bDrawing = true;
}

/// <summary>
/// 마지막 점과 이을 새로운 점 추가, 초기화 직후라면 첫 점으로 설정
/// </summary>
/// <param name="Point"></param>
void ULineDrawingComponent::AddPoint(FVector Point)
{
	if (DrawingTick > DrawingInterval)
	{
		SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
		UpdateSpline();

		DrawingTick = 0;
	}
}

/// <summary>
/// 
/// </summary>
void ULineDrawingComponent::EndDrawing()
{
	bDrawing = false;
}

/// <summary>
/// 새로 추가된 점과 이전 점을 잇는 Spline Mesh 추가
/// </summary>
void ULineDrawingComponent::UpdateSpline()
{
	if (!DefaultMesh || !DefaultMaterial)
	{
		return;
	}

	// Spline Mesh를 생성하기에 Point가 부족하거나 이미 필요한 Spline Mesh가 모두 생성된 경우 종료
	int32 SplinePointNum = SplineComponent->GetNumberOfSplinePoints();

	if (SplinePointNum <= 1 || SplinePointNum - 1 == SplineMeshes.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Spline Point가 부족하거나 Spline Mesh가 모두 생성된 상태에서 호출되었습니다."));
		return;
	}

	/* Spline Mesh 생성 및 설정 */
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);	// Spline Mesh 정면 방향 설정
	SplineMesh->SetStaticMesh(DefaultMesh);			// Spline Mesh로 사용할 Static Mesh 설정
	SplineMesh->SetMobility(EComponentMobility::Movable);
	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SplineMesh->SetStartScale(FVector2D(0.1f, 0.1f));
	SplineMesh->SetEndScale(FVector2D(0.1f, 0.1f));
	SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SplineMesh->RegisterComponentWithWorld(GetWorld());

	// Spline Mesh Point와 Tangent 설정
	// Spline Mesh는 Start, End에 대한 Point와 Tangent로 구성되어 Mesh를 변형
	int32 CurrentSplinePointIndex = SplinePointNum - 1;
	int32 PreviousSplinePointIndex = SplinePointNum - 2;
	const FVector SplineMeshStartPoint = SplineComponent->GetLocationAtSplinePoint(PreviousSplinePointIndex, ESplineCoordinateSpace::Local);
	const FVector SplineMeshStartTangent = SplineComponent->GetTangentAtSplinePoint(PreviousSplinePointIndex, ESplineCoordinateSpace::Local);
	const FVector SplineMeshEndPoint = SplineComponent->GetLocationAtSplinePoint(CurrentSplinePointIndex, ESplineCoordinateSpace::Local);
	const FVector SplineMeshEndTangent = SplineComponent->GetTangentAtSplinePoint(CurrentSplinePointIndex, ESplineCoordinateSpace::Local);
	SplineMesh->SetStartAndEnd(SplineMeshStartPoint, SplineMeshStartTangent, SplineMeshEndPoint, SplineMeshEndTangent);
	SplineMesh->SetMaterial(0, DefaultMaterial);

	SplineMeshes.Add(SplineMesh);
}
