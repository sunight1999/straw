// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"

class UPhysicsHandleComponent;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRAW_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab();
	void Release();

protected:
	virtual void BeginPlay() override;

private:
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 300;

	UPROPERTY(EditAnywhere, Category="Grabber")
	float GrabRadius = 100;

	UPROPERTY(EditAnywhere, Category = "Grabber")
	float HoldDistance = 100;

	ABaseCharacter* Owner;
};
