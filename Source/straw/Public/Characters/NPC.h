// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

struct FQuestDetail;
class UQuestSubsystem;

UCLASS()
class STRAW_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	ANPC();
	void Talk();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "NPC")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="NPC")
	TArray<FString> QuestIDs;

	UQuestSubsystem* QuestSubsystem;
	TMap<FString, FQuestDetail*> QuestsMap;
};
