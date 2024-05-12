// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicInteraction.generated.h"

class USceneComponent;
class UBoxComponent;
class UWidgetComponent;
class ABaseCharacter;

UCLASS()
class STRAW_API ABasicInteraction : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasicInteraction();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "BasicInteraction")
	class UBoxComponent* OverlapBox;

	UPROPERTY(VisibleAnywhere, Category = "BasicInteraction")
	class UWidgetComponent* InteractableWidget;

	ABaseCharacter* Player;

private:
};
