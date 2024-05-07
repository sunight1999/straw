// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/BasicInteraction.h"
#include "Components/BoxComponent.h"
#include "Characters/BaseCharacter.h"

// Sets default values
ABasicInteraction::ABasicInteraction()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;
}

// Called when the game starts or when spawned
void ABasicInteraction::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ABasicInteraction::OnBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ABasicInteraction::OnBoxEndOverlap);
}

void ABasicInteraction::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ABaseCharacter>(OtherActor);

	if (Player)
	{
		Player->SetInteraction(this);
	}
}

void ABasicInteraction::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player)
	{
		Player->ReleaseInteraction();
	}
}

// Called every frame
void ABasicInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

