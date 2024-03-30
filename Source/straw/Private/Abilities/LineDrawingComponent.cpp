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
/// ������ �׷ȴ� ������ ��� �ʱ�ȭ
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

	DrawingTick = DrawingInterval + 1;	// ���� ������ ���� ���� �ٷ� �׸� �� �ֵ��� DrawingInterval�� 1�� ���� ������ �ʱ�ȭ
	bDrawing = true;
}

/// <summary>
/// ������ ���� ���� ���ο� �� �߰�, �ʱ�ȭ ���Ķ�� ù ������ ����
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
/// ���� �߰��� ���� ���� ���� �մ� Spline Mesh �߰�
/// </summary>
void ULineDrawingComponent::UpdateSpline()
{
	if (!DefaultMesh || !DefaultMaterial)
	{
		return;
	}

	// Spline Mesh�� �����ϱ⿡ Point�� �����ϰų� �̹� �ʿ��� Spline Mesh�� ��� ������ ��� ����
	int32 SplinePointNum = SplineComponent->GetNumberOfSplinePoints();

	if (SplinePointNum <= 1 || SplinePointNum - 1 == SplineMeshes.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Spline Point�� �����ϰų� Spline Mesh�� ��� ������ ���¿��� ȣ��Ǿ����ϴ�."));
		return;
	}

	/* Spline Mesh ���� �� ���� */
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);	// Spline Mesh ���� ���� ����
	SplineMesh->SetStaticMesh(DefaultMesh);			// Spline Mesh�� ����� Static Mesh ����
	SplineMesh->SetMobility(EComponentMobility::Movable);
	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SplineMesh->SetStartScale(FVector2D(0.1f, 0.1f));
	SplineMesh->SetEndScale(FVector2D(0.1f, 0.1f));
	SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SplineMesh->RegisterComponentWithWorld(GetWorld());

	// Spline Mesh Point�� Tangent ����
	// Spline Mesh�� Start, End�� ���� Point�� Tangent�� �����Ǿ� Mesh�� ����
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
