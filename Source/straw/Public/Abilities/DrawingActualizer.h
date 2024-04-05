// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawingActualizer.generated.h"

class UProceduralMeshComponent;

UCLASS()
class STRAW_API ADrawingActualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawingActualizer();
	virtual void Tick(float DeltaTime) override;

	void Actualize2D(TArray<FVector> DrawingVertices, FBox DrawingPlaneBox, FRotator DrawingPlaneRotation, UMaterialInterface* MeshMaterial);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProcMeshComponent;

	UMaterialInterface* Material;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
};
