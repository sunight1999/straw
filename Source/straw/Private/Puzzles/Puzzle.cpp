// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/Puzzle.h"
#include "Components/BoxComponent.h"

// Sets default values
APuzzle::APuzzle()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;
}

void APuzzle::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &APuzzle::OnBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &APuzzle::OnBoxEndOverlap);
}

void APuzzle::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BeginPuzzle();
}

void APuzzle::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void APuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

