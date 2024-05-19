// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/BasicInteraction.h"
#include "Interacts/Rootable.h"
#include "TraditionalOrnament.generated.h"

UENUM(BlueprintType)
enum class EOrnamentPart : uint8
{
	LeftTop,
	LeftBottom,
	RightBottom,
	RightTop
};

/**
 * 
 */
UCLASS()
class STRAW_API ATraditionalOrnament : public ABasicInteraction, public IRootable
{
	GENERATED_BODY()
	
public:
	ATraditionalOrnament();

	EOrnamentPart GetOrnamentPart() const;

	void Root();
	
private:
	UPROPERTY(EditAnywhere, Category = "TraditionalOrnament")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "TraditionalOrnament")
	EOrnamentPart OrnamentPart = EOrnamentPart::LeftTop;
};
