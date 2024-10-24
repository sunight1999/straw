// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawingAbilityComponent.generated.h"

class UMaterialInterface;
class UDecalComponent;
class USplineComponent;
class USplineMeshComponent;
class UBoxComponent;
class UNiagaraComponent;
class ABaseCharacter;
class AHanji;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STRAW_API UDrawingAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrawingAbilityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartDrawing();
	bool AddPointDirty(FVector& OutImpactPoint);
	void AddPoint(FVector Point, bool bForceDraw = false);
	void EndDrawing();

protected:
	virtual void BeginPlay() override;

private:
	void UpdateSpline();

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	UNiagaraComponent* AbilityGainedEffect;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float AbilityGainedEffectZOffset;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	bool bDrawByDistance = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawing Ability", Meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;
	TArray<USplineMeshComponent*> SplineMeshes;
	TArray<FVector> SplinePoints;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	UStaticMesh* LineMesh;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	UMaterialInterface* LineMaterial;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	UMaterialInterface* ActualizedObjectMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Drawing Ability")
	UNiagaraComponent* ActualizedEffect;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float ActualizedObjectThickness = 30.f;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingDistance = 300.f;

	// bDrawByDistance가 false일 때만 사용
	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingInterval = 0.04f;
	float DrawingTick = 0;

	// bDrawByDistance가 true일 때만 사용
	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingGap = 15.f;
	FVector PreviousPoint;

	AHanji* Hanji = nullptr;
	UBoxComponent* DrawingCollision = nullptr;
	bool bDrawing;

	ABaseCharacter* OwnerCharacter;	// 카메라 회전 값을 가져올 때 사용할 캐릭터 포인터 캐싱
};
