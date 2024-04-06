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
class ABaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STRAW_API UDrawingAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrawingAbilityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartDrawing();
	void AddPoint();
	void AddPoint(FVector Point, bool bForceDraw = false);
	void EndDrawing();

protected:
	virtual void BeginPlay() override;

private:
	void UpdateSpline();

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	UBoxComponent* DrawingCollision;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingCollisionDistance = 150.f;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingCollisionHeight = 180.f;

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

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float ActualizedObjectThickness = 30.f;

	UPROPERTY(EditAnywhere, Category = "Drawing Ability")
	float DrawingInterval = 0.04f;
	float DrawingTick = 0;

	bool bDrawing;

	ABaseCharacter* OwnerCharacter;	// 카메라 회전 값을 가져올 때 사용할 캐릭터 포인터 캐싱
	//UPROPERTY(VisibleAnywhere, Category = "Drawing Ability")
	//UDecalComponent* CircleDecal;
};
