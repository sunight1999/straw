// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineDrawingComponent.generated.h"

class UMaterialInterface;
class UDecalComponent;
class USplineComponent;
class USplineMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STRAW_API ULineDrawingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULineDrawingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartDrawing();
	void AddPoint(FVector Point);
	void EndDrawing();

protected:
	virtual void BeginPlay() override;

private:
	void UpdateSpline();

	UPROPERTY(VisibleAnywhere, Category = "Line Drawing")
	USplineComponent* SplineComponent;
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditAnywhere, Category = "Line Drawing")
	UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, Category = "Line Drawing")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Line Drawing")
	float DrawingInterval = 0.5f;
	float DrawingTick = 0;

	bool bDrawing;

	//UPROPERTY(VisibleAnywhere, Category = "Line Drawing")
	//UDecalComponent* CircleDecal;
};
