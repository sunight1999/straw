// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/BasicInteraction.h"
#include "Interacts/Collectable.h"
#include "TraditionalKey.generated.h"

class ATraditionalBox;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class STRAW_API ATraditionalKey : public ABasicInteraction, public ICollectable
{
	GENERATED_BODY()
	
public:
	ATraditionalKey();
	void Tick(float DeltaTime) override;
	
	void Collect();
	void Use(ATraditionalBox* TraditionalBox);

	const FString GetKeyID() const { return KeyID; };
	float GetKeyLength() const;

private:
	UPROPERTY(EditAnywhere, Category = "TraditionalKey")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "TraditionalKey")
	UNiagaraComponent* TrackingEffect;

	UPROPERTY(EditAnywhere, Category = "TraditionalKey")
	float TrackingSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "TraditionalKey")
	float TrackingPadding = 20.f;

	UPROPERTY(EditAnywhere, Category = "TraditionalKey")
	FString KeyID;

	bool bIsTracking = false;
};
