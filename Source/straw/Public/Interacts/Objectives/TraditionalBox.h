// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interacts/BasicInteraction.h"
#include "Interacts/Interactable.h"
#include "TraditionalBox.generated.h"

class ATraditionalKey;
class ATraditionalOrnament;
class ULockCheckUI;
class ULockCheckSubsystem;

/**
 * 
 */
UCLASS()
class STRAW_API ATraditionalBox : public ABasicInteraction, public IInteractable
{
	GENERATED_BODY()
public:
	ATraditionalBox();

	void Interact();
	void Unlock(ATraditionalKey* Key);

protected:
	void BeginPlay() override;

private:
	void HandleUnlock(bool bResult);

	UPROPERTY(EditAnywhere, Category = "TraditionalBox")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "TraditionalBox")
	FString KeyID;

	UPROPERTY(EditAnywhere, Category = "TraditionalBox")
	TSubclassOf<ATraditionalOrnament> OrnamentClass;

	ATraditionalOrnament* TraditionalOrnament;
	bool bIsUnlocked = false;

	ATraditionalKey* CurrentKey = nullptr;
	ULockCheckSubsystem* LockCheckSubsystem;
};
