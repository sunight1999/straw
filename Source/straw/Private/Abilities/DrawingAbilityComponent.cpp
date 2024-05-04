// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DrawingAbilityComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Abilities/DrawingActualizer.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Characters/BaseCharacter.h"

UDrawingAbilityComponent::UDrawingAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AbilityGainedEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AbilityGainedEffect"));
	AbilityGainedEffect->bAutoActivate = false;

	DrawingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DrawingCollision"));
	DrawingCollision->SetBoxExtent(FVector(1.f, 300.f, 100.f));
	DrawingCollision->SetRelativeLocation(FVector(80.f, 0.f, 80.f));

	// 현재 collision channel 설정이 안 됨
	DrawingCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DrawingCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	DrawingCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DrawingCollision->bHiddenInGame = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	ActualizedEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ActualizedEffect"));
	ActualizedEffect->bAutoActivate = false;
}

void UDrawingAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());

	// 능력 획득 이펙트 재생, 추후 도깨비와의 대화 이후 얻을 수 있게 코드 위치 이동 예정
	USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
	FVector EffectLocation = CharacterMesh->GetComponentLocation();
	EffectLocation.Z += CharacterMesh->GetRelativeLocation().Z + AbilityGainedEffectZOffset;

	AbilityGainedEffect->SetWorldLocation(EffectLocation);
	AbilityGainedEffect->Activate();
}

void UDrawingAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bDrawing)
	{
		if (!bDrawByDistance)
		{
			DrawingTick += DeltaTime;
		}
	}
}

/*
 * 그리기 관련 함수
 */
/// <summary>
/// 이전에 그렸던 정보를 모두 초기화
/// </summary>
void UDrawingAbilityComponent::StartDrawing()
{
	// Drawing Collision 위치 및 회전값 설정
	FRotator CameraRotation;
	const FVector CameraForwardVector = OwnerCharacter->GetCameraForwardVector(&CameraRotation, true);
	FVector NewLocation = OwnerCharacter->GetActorLocation() + CameraForwardVector * DrawingCollisionDistance;
	NewLocation.Z += DrawingCollisionHeight;
	CameraRotation.Pitch = 0.f;

	DrawingCollision->SetWorldLocationAndRotation(NewLocation, CameraRotation);
	DrawingCollision->SetVisibility(true);
	DrawingCollision->bHiddenInGame = false;

	// 이전 Drawing 정보 초기화
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

	DrawingTick = DrawingInterval + 1;	// 시작 지점을 지연 없이 바로 그릴 수 있도록 DrawingInterval에 1을 더한 값으로 초기화
	bDrawing = true;
}

/// <summary>
/// 선을 구성할 새로운 점 추가, 현재 카메라 Rotation 방향으로 Line Trace를 수행해 Drawing Collision과 충돌한 지점을 새로운 점으로 지정
/// </summary>
void UDrawingAbilityComponent::AddPoint()
{
	if (!bDrawByDistance)
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
	else
	{
		FHitResult OutHit;

		FVector StartPosition = OwnerCharacter->GetActorLocation();
		FVector EndPosition = StartPosition + DrawingCollisionDistance * 10.f * OwnerCharacter->GetCameraForwardVector(nullptr, true);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartPosition, EndPosition, ECC_GameTraceChannel1))
		{
			if (SplineComponent->GetNumberOfSplinePoints() == 0 || FVector::DistSquared(PreviousPoint, OutHit.ImpactPoint) >= DrawingGap * DrawingGap)
			{
				AddPoint(OutHit.ImpactPoint, true);
				PreviousPoint = OutHit.ImpactPoint;
			}
		}
	}
}

/// <summary>
/// Point 추가 간격과 별개로 ImpactPoint를 계속 반환하여 이펙트 Point를 지정할 수 있도록 함
/// </summary>
/// <param name="OutImpactPoint">Drawing Collision 충돌 지점</param>
/// <returns>충돌점 존재 여부</returns>
bool UDrawingAbilityComponent::AddPointDirty(FVector& OutImpactPoint)
{
	FHitResult OutHit;

	FVector StartPosition = OwnerCharacter->GetActorLocation();
	FVector EndPosition = StartPosition + DrawingCollisionDistance * 10.f * OwnerCharacter->GetCameraForwardVector(nullptr, true);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartPosition, EndPosition, ECC_GameTraceChannel1))
	{
		OutImpactPoint = OutHit.ImpactPoint;

		if (!bDrawByDistance)
		{
			if (DrawingTick > DrawingInterval)
			{
				AddPoint(OutHit.ImpactPoint, true);
			}
		}
		else
		{
			if (SplineComponent->GetNumberOfSplinePoints() == 0 || FVector::DistSquared(PreviousPoint, OutHit.ImpactPoint) >= DrawingGap * DrawingGap)
			{
				AddPoint(OutHit.ImpactPoint, true);
				PreviousPoint = OutHit.ImpactPoint;
			}
		}

		return true;
	}

	return false;
}

/// <summary>
/// 선을 구성할 새로운 점 추가
/// </summary>
/// <param name="Point">새로운 점 Vector 정보</param>
/// <param name="bForceDraw">true일 경우 Drawing Interval과 상관없이 무조건 점 추가, Drawing Tick은 초기화 됨</param>
void UDrawingAbilityComponent::AddPoint(FVector Point, bool bForceDraw)
{
	if (bForceDraw ||
		(!bDrawByDistance && (DrawingTick > DrawingInterval)) ||
		(bDrawByDistance && (SplineComponent->GetNumberOfSplinePoints() == 0 || FVector::DistSquared(PreviousPoint, Point) >= DrawingGap * DrawingGap)))
	{
		SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
		SplinePoints.Add(Point);

		UpdateSpline();

		DrawingTick = 0;
	}
}

/// <summary>
/// 선 그리기를 종료하고 오브젝트 생성 시작
/// </summary>
void UDrawingAbilityComponent::EndDrawing()
{
	DrawingCollision->SetVisibility(false);
	DrawingCollision->bHiddenInGame = true;
	bDrawing = false;
	
	// DrawingActualizer에 선 버텍스 정보를 전달해 오브젝트 생성 요청
	ADrawingActualizer* DrawingActualizer = GetWorld()->SpawnActor<ADrawingActualizer>(ADrawingActualizer::StaticClass(), FVector::ZeroVector, DrawingCollision->GetComponentRotation());
	FVector CenterPosition = DrawingActualizer->Actualize2D(SplinePoints, DrawingCollision->Bounds.GetBox(), DrawingCollision->GetComponentRotation(), ActualizedObjectThickness, ActualizedObjectMaterial);

	ActualizedEffect->DeactivateImmediate();
	ActualizedEffect->SetWorldLocation(CenterPosition);
	ActualizedEffect->Activate();
}

/// <summary>
/// 새로 추가된 점과 이전 점을 잇는 Spline Mesh 추가
/// </summary>
void UDrawingAbilityComponent::UpdateSpline()
{
	if (!LineMesh || !LineMaterial)
	{
		return;
	}

	// Spline Mesh를 생성하기에 Point가 부족하거나 이미 필요한 Spline Mesh가 모두 생성된 경우 종료
	int32 SplinePointNum = SplineComponent->GetNumberOfSplinePoints();

	if (SplinePointNum <= 1 || SplinePointNum - 1 == SplineMeshes.Num())
	{
		return;
	}

	/* Spline Mesh 생성 및 설정 */
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);	// Spline Mesh 정면 방향 설정
	SplineMesh->SetStaticMesh(LineMesh);			// Spline Mesh로 사용할 Static Mesh 설정
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
	SplineMesh->SetMaterial(0, LineMaterial);

	SplineMeshes.Add(SplineMesh);
}
