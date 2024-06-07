// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Chapter1/Hanji.h"
#include "Components/BoxComponent.h"
#include "Interacts/SplineMover.h"

AHanji::AHanji()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);

	DrawingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DrawingCollision"));
	DrawingCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DrawingCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	DrawingCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//DrawingCollision->bHiddenInGame = true;
	DrawingCollision->SetupAttachment(StaticMesh);

	SplineMover = CreateDefaultSubobject<USplineMover>(TEXT("SplineMover"));
}

void AHanji::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHanji::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHanji::Play()
{
	SplineMover->Play();
}

void AHanji::SetTarget(AActor* Target)
{
	SplineMover->SetTarget(Target);
}

