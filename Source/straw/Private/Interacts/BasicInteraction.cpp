// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/BasicInteraction.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Characters/BaseCharacter.h"

// Sets default values
ABasicInteraction::ABasicInteraction()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;

	InteractableWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidget"));
	InteractableWidget->SetupAttachment(RootComponent);
	InteractableWidget->SetVisibility(false);

	// 상호작용 가능 UI를 기본값으로 할당
	ConstructorHelpers::FClassFinder<UUserWidget> InteractableWidgetFinder(TEXT("/Game/UI/W_InteractivePrompt"));
	if (InteractableWidgetFinder.Succeeded())
	{
		InteractableWidget->SetWidgetClass(InteractableWidgetFinder.Class);
	}
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
	InteractableWidget->SetVisibility(true);

	Player = Cast<ABaseCharacter>(OtherActor);

	if (Player)
	{
		Player->SetInteraction(this);
	}
}

void ABasicInteraction::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InteractableWidget->SetVisibility(false);

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

