// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/BasicInteraction.h"
#include "Interacts/Interactable.h"
#include "Pottery.generated.h"

class APotteryPuzzle;

/**
 * 
 */
UCLASS()
class STRAW_API APottery : public ABasicInteraction, public IInteractable
{
	GENERATED_BODY()
	
public:
	APottery();

	void Interact() override;
	void PlayAnimation();
	float GetAnimationLength();

	void SetPotteryPuzzle(APotteryPuzzle* InPotteryPuzzle);
	void SetPotteryID(int32 ID);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Pottery")
	USkeletalMeshComponent* SkeletalMesh;

	APotteryPuzzle* PotteryPuzzle;
	int32 PotteryID;	// PotteryPuzzle에서 부여하는 ID
	float AnimationLength = 0.f;
};
