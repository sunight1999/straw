// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Objectives/TraditionalKey.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Characters/BaseCharacter.h"

ATraditionalKey::ATraditionalKey()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);
}

void ATraditionalKey::Tick(float DeltaTime)
{
	if (bIsTracking)
	{
		FVector TargetLocation = Player->GetActorLocation();
		TargetLocation += -(Player->GetTraditionalKeyOffset(KeyID) + TrackingPadding) * Player->GetActorForwardVector();
		FVector Location = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, TrackingSpeed);

		FRotator TargetRotation = Player->GetActorRotation();
		FRotator Rotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TrackingSpeed);

		SetActorLocationAndRotation(Location, Rotation);
	}
}

void ATraditionalKey::Collect()
{
	StopInteraction();

	// 플레이어 추적 시작
	bIsTracking = true;
}

void ATraditionalKey::Use(ATraditionalBox* TraditionalBox)
{
	// 자물쇠 여는 애니메이션 재생
	
	// 열쇠 사라지는 애니메이션 재생
	
	Destroy();
}

float ATraditionalKey::GetKeyLength() const
{
	FVector MeshExtent = Mesh->Bounds.BoxExtent;
	return FMath::Max3<float>(MeshExtent.X, MeshExtent.Y, MeshExtent.Z);
}
