// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puzzle.generated.h"

UCLASS(Abstract)
class STRAW_API APuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	APuzzle();
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPuzzle() PURE_VIRTUAL(APuzzle::BeginPuzzle, ;);
	virtual void EndPuzzle() PURE_VIRTUAL(APuzzle::EndPuzzle, ;);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "BasicInteraction")
	class UBoxComponent* OverlapBox;

private:	
	

};
