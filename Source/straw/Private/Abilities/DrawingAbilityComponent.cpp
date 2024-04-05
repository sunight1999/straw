// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DrawingAbilityComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Abilities/DrawingActualizer.h"
#include "Components/BoxComponent.h"
#include "Characters/BaseCharacter.h"

#include "Math/StrawTriangle.h"

UDrawingAbilityComponent::UDrawingAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DrawingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DrawingCollision"));
	DrawingCollision->SetBoxExtent(FVector(1.f, 300.f, 100.f));
	DrawingCollision->SetRelativeLocation(FVector(80.f, 0.f, 80.f));

	// ���� collision channel ������ �� ��
	DrawingCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DrawingCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	DrawingCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DrawingCollision->bHiddenInGame = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void UDrawingAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

void UDrawingAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bDrawing)
	{
		DrawingTick += DeltaTime;
	}
}

/*
 * �׸��� ���� �Լ�
 */
/// <summary>
/// ������ �׷ȴ� ������ ��� �ʱ�ȭ
/// </summary>
void UDrawingAbilityComponent::StartDrawing()
{
	// Drawing Collision ��ġ �� ȸ���� ����
	FRotator CameraRotation;
	const FVector CameraForwardVector = OwnerCharacter->GetCameraForwardVector(&CameraRotation, true);
	FVector NewLocation = OwnerCharacter->GetActorLocation() + CameraForwardVector * DrawingCollisionDistance;
	NewLocation.Z = DrawingCollisionHeight;

	DrawingCollision->SetWorldLocationAndRotation(NewLocation, CameraRotation);
	DrawingCollision->bHiddenInGame = false;

	// ���� Drawing ���� �ʱ�ȭ
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
	SplinePoints.Empty();

	DrawingTick = DrawingInterval + 1;	// ���� ������ ���� ���� �ٷ� �׸� �� �ֵ��� DrawingInterval�� 1�� ���� ������ �ʱ�ȭ
	bDrawing = true;
}

/// <summary>
/// ���� ������ ���ο� �� �߰�, ���� ī�޶� Rotation �������� Line Trace�� ������ Drawing Collision�� �浹�� ������ ���ο� ������ ����
/// </summary>
void UDrawingAbilityComponent::AddPoint()
{
	if (DrawingTick > DrawingInterval)
	{
		FHitResult OutHit;

		FVector StartPosition = OwnerCharacter->GetActorLocation();
		FVector EndPosition = StartPosition + DrawingCollisionDistance * 10.f * OwnerCharacter->GetCameraForwardVector(nullptr, true);
		
		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartPosition, EndPosition, ECC_GameTraceChannel1))
		{
			AddPoint(OutHit.ImpactPoint, true);
		}
	}
}

/// <summary>
/// ���� ������ ���ο� �� �߰�
/// </summary>
/// <param name="Point">���ο� �� Vector ����</param>
/// <param name="bForceDraw">true�� ��� Drawing Interval�� ������� ������ �� �߰�, Drawing Tick�� �ʱ�ȭ ��</param>
void UDrawingAbilityComponent::AddPoint(FVector Point, bool bForceDraw)
{
	if (bForceDraw || DrawingTick > DrawingInterval)
	{
		SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
		SplinePoints.Add(Point);

		UpdateSpline();

		DrawingTick = 0;
	}
}

/// <summary>
/// �� �׸��⸦ �����ϰ� ������Ʈ ���� ����
/// </summary>
void UDrawingAbilityComponent::EndDrawing()
{
	DrawingCollision->bHiddenInGame = true;
	bDrawing = false;
	
	// DrawingActualizer�� �� ���ؽ� ������ ������ ������Ʈ ���� ��û
	ADrawingActualizer* DrawingActualizer = GetWorld()->SpawnActor<ADrawingActualizer>(ADrawingActualizer::StaticClass(), GetOwner()->GetActorLocation(), FQuat::Identity.Rotator());
	DrawingActualizer->Actualize2D(SplinePoints, DrawingCollision->Bounds.GetBox(), DrawingCollision->GetComponentRotation(), ActualizedObjectMaterial);
}

/// <summary>
/// ���� �߰��� ���� ���� ���� �մ� Spline Mesh �߰�
/// </summary>
void UDrawingAbilityComponent::UpdateSpline()
{
	if (!LineMesh || !LineMaterial)
	{
		return;
	}

	// Spline Mesh�� �����ϱ⿡ Point�� �����ϰų� �̹� �ʿ��� Spline Mesh�� ��� ������ ��� ����
	int32 SplinePointNum = SplineComponent->GetNumberOfSplinePoints();

	if (SplinePointNum <= 1 || SplinePointNum - 1 == SplineMeshes.Num())
	{
		return;
	}

	/* Spline Mesh ���� �� ���� */
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);	// Spline Mesh ���� ���� ����
	SplineMesh->SetStaticMesh(LineMesh);			// Spline Mesh�� ����� Static Mesh ����
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
	SplineMesh->SetMaterial(0, LineMaterial);

	SplineMeshes.Add(SplineMesh);
}
