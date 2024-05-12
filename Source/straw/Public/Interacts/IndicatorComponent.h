// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IndicatorComponent.generated.h"

class UIndicatorUI;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRAW_API UIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIndicatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Indicator")
	UIndicatorUI* IndicatorWidget;

	UPROPERTY(EditAnywhere, Category="Indicator")
	float IndicatorOffset;

	APlayerController* PlayerController;
	AActor* Owner;
};
