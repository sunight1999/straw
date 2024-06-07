// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hanji.generated.h"

class UBoxComponent;
class USplineMover;

UCLASS()
class STRAW_API AHanji : public AActor
{
	GENERATED_BODY()
	
public:
	AHanji();
	virtual void Tick(float DeltaTime) override;

	void Play();
	void SetTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Hanji")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, Category = "Hanji")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Hanji")
	UBoxComponent* DrawingCollision;

	UPROPERTY(EditAnywhere, Category = "Hanji")
	USplineMover* SplineMover;
};
