// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRAW_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMover();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, Category = "Mover")
	float Velocity = 100;

private:
	virtual void Move(float DeltaTime);
	virtual void Rotate(float DeltaTime);

	UPROPERTY(EditAnyWhere, Category = "Mover")
	FVector From;

	UPROPERTY(EditAnyWhere, Category = "Mover")
	FVector To;
};
