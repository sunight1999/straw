// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Objectives/TraditionalBox.h"
#include "Components/WidgetComponent.h"

#include "Interacts/Objectives/TraditionalKey.h"
#include "Interacts/Objectives/TraditionalOrnament.h"
#include "Characters/BaseCharacter.h"

ATraditionalBox::ATraditionalBox()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->AnimationData.bSavedLooping = false;
	SkeletalMesh->AnimationData.bSavedPlaying = false;
}

void ATraditionalBox::BeginPlay()
{
	Super::BeginPlay();

	verify(OrnamentClass != nullptr)

	TraditionalOrnament = GetWorld()->SpawnActor<ATraditionalOrnament>(OrnamentClass, SkeletalMesh->GetComponentLocation(), GetActorRotation());
	TraditionalOrnament->SetActorHiddenInGame(true);
}

void ATraditionalBox::Interact()
{
	if (bIsUnlocked)
	{
		return;
	}

	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어의 정보가 존재하지 않습니다."));
		return;
	}

	ATraditionalKey* Key = Player->HasTraditionalKey(KeyID);
	if (!Key)
	{
		// 전통함 또는 자물쇠가 덜컹거리는 애니메이션 + 소리 재생
		return;
	}

	Unlock(Key);
}

void ATraditionalBox::Unlock(ATraditionalKey* Key)
{
	if (Key->GetKeyID().Compare(KeyID) == 0)
	{
		bIsUnlocked = true;

		StopInteraction();

		// 열쇠 애니메이션 재생 후 Destroy
		Key->Use(this);
		Player->UseTraditionalKey(KeyID);
		
		// 전통함 열리는 애니메이션 재생
		SkeletalMesh->Play(false);

		// 문양 조각 뱉기
		TraditionalOrnament->SetActorHiddenInGame(false);

		// 문양 조각 애니메이션 재생, 추후 playing과 loop를 true로 바꿔서 애니메이션 재생시키기 (현재 애니메이션이 없어 임시 코드 수행)
		FVector TargetLocation = TraditionalOrnament->GetActorLocation();
		TargetLocation.Z += 70.f;
		
		TraditionalOrnament->SetActorLocation(TargetLocation);
	}
}
