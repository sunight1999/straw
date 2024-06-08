// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Abilities/DrawingAbilityComponent.h"
#include "Interacts/Grabber.h"
#include "Interacts/Interactable.h"
#include "Interacts/Collectable.h"
#include "Interacts/Rootable.h"
#include "Interacts/Objectives/TraditionalKey.h"
#include "Interacts/Objectives/TraditionalOrnament.h"
#include "Quests/QuestSubsystem.h"
#include "Dialogues/DialogueSubsystem.h"
#include "SubGame/LockCheckSubsystem.h"
#include "HUD/MainHUD.h"
#include "HUD/OrnamentOverlay.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 300.f;
	CameraArm->bUsePawnControlRotation = true;	// 캐릭터와 컨트롤러의 회전을 별개로 구분

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraArm);

	//PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));
	Grabber->SetupAttachment(GetRootComponent());

	AbilityEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AbilityEffectComponent"));
	AbilityEffectComponent->bAutoActivate = false;

	DrawingAbilityComponent = CreateDefaultSubobject<UDrawingAbilityComponent>(TEXT("DrawingAbilityComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터를 움직이고 있는 방향으로 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// HUD 캐싱
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
	}

	// 전통 문양 조각 UI 캐싱
	OrnamentOverlay = MainHUD->GetOrnamentOverlay();
	OrnamentOverlay->SetVisibility(ESlateVisibility::Hidden);

	// 퀘스트 UI 초기화
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>())
		{
			QuestSubsystem->UpdateUI();
		}

		DialogueSubsystem = GameInstance->GetSubsystem<UDialogueSubsystem>();
		LockCheckSubsystem = GameInstance->GetSubsystem<ULockCheckSubsystem>();
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bReadyAbility && bUsingAbility)
	{
		FVector ImpactPoint;
		if (DrawingAbilityComponent->AddPointDirty(ImpactPoint))
		{
			// 먹 이펙트 재생
			if (!AbilityEffectComponent->IsActive() && AbilityEffectComponent->GetAsset())
			{
				AbilityEffectComponent->Activate();
			}
			AbilityEffectComponent->SetVariablePosition(FName("SpawnPosition"), ImpactPoint);
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(FName("Grab"), IE_Pressed, this, &ABaseCharacter::TryGrab);
	PlayerInputComponent->BindAction(FName("Grab"), IE_Released, this, &ABaseCharacter::TryRelease);
	PlayerInputComponent->BindAction(FName("ReadyAbility"), IE_Pressed, this, &ABaseCharacter::ReadyAbility);
	PlayerInputComponent->BindAction(FName("ReadyAbility"), IE_Released, this, &ABaseCharacter::EndReadyAbility);
	PlayerInputComponent->BindAction(FName("UseAbility"), IE_Pressed, this, &ABaseCharacter::UseAbility);
	PlayerInputComponent->BindAction(FName("UseAbility"), IE_Released, this, &ABaseCharacter::EndUseAbility);
	PlayerInputComponent->BindAction(FName("Action"), IE_Pressed, this, &ABaseCharacter::Action);
}

FVector ABaseCharacter::GetCameraForwardVector(FRotator* OutCameraRotation, bool bIncludePitch) const
{
	const FRotator ControlRotation = GetControlRotation();
	FRotator Rotation(ControlRotation);
	if (!bIncludePitch)
	{
		Rotation.Pitch = 0.f;
	}
	
	const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

	if (OutCameraRotation)
	{
		*OutCameraRotation = ControlRotation;
	}

	return Direction;
}

void ABaseCharacter::SetInteraction(AActor* Actor)
{
	if (UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractable::StaticClass()))
	{
		CurrentInteraction = Actor;
	}
	else if (UKismetSystemLibrary::DoesImplementInterface(Actor, UCollectable::StaticClass()))
	{
		CurrentInteraction = Actor;
	}
	else if (UKismetSystemLibrary::DoesImplementInterface(Actor, URootable::StaticClass()))
	{
		SetRootable(Actor);
	}
}

void ABaseCharacter::ReleaseInteraction()
{
	CurrentInteraction = nullptr;
}

void ABaseCharacter::SetCollectable(AActor* Actor)
{
	if (ATraditionalKey* Key = Cast<ATraditionalKey>(Actor))
	{
		TraditionalKeys.Add(Key);
	}
}

void ABaseCharacter::SetRootable(AActor* Actor)
{
	if (ATraditionalOrnament* TraditionalOrnament = Cast<ATraditionalOrnament>(Actor))
	{
		OrnamentOverlay->SetVisibility(ESlateVisibility::Visible);

		EOrnamentPart OrnamentPart = TraditionalOrnament->GetOrnamentPart();
		
		TraditionalOrnaments[static_cast<int8>(OrnamentPart)] = true;
		OrnamentOverlay->SetOrnamentVisibility(OrnamentPart, true);
	}

	IRootable* Rootable = Cast<IRootable>(Actor);
	Rootable->Root();
}

ATraditionalKey* ABaseCharacter::HasTraditionalKey(FString KeyID)
{
	for (auto Key : TraditionalKeys)
	{
		if (Key->GetKeyID().Compare(KeyID) == 0)
		{
			return Key;
		}
	}

	return nullptr;
}

float ABaseCharacter::GetTraditionalKeyOffset(FString KeyID)
{
	if (float* offset = TraditionalKeyOffsetMap.Find(KeyID))
	{
		return *offset;
	}
	
	float offset = 0.f;
	for (auto Key : TraditionalKeys)
	{
		if (Key->GetKeyID().Compare(KeyID) == 0)
		{
			TraditionalKeyOffsetMap.Add(KeyID, offset);
			return offset;
		}

		offset += Key->GetKeyLength();
	}

	UE_LOG(LogTemp, Error, TEXT("존재하지 않는 키 정보입니다. (%s)"), *KeyID);
	return 0.f;
}

void ABaseCharacter::UseTraditionalKey(FString KeyID)
{
	for (int i = 0; i < TraditionalKeys.Num(); i++)
	{
		if (TraditionalKeys[i]->GetKeyID().Compare(KeyID) == 0)
		{
			ATraditionalKey* Key = TraditionalKeys[i];
			TraditionalKeys.RemoveAt(i);
			TraditionalKeyOffsetMap.Empty(); // 모든 키의 위치 offset을 재계산

			return;
		}
	}
}

bool ABaseCharacter::IsTraditionalOrnamentReady()
{
	for (bool bCollected : TraditionalOrnaments)
	{
		if (!bCollected)
		{
			return false;
		}
	}

	return true;
}

void ABaseCharacter::MoveForward(float Value)
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (Controller && (Value != 0.f))
	{
		AddMovementInput(GetCameraForwardVector(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Turn(float Value)
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	AddControllerYawInput(Value);
}

void ABaseCharacter::LookUp(float Value)
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	AddControllerPitchInput(Value);
}

void ABaseCharacter::TryGrab()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (!bReadyAbility)
	{
		Grabber->Grab();
	}
}

void ABaseCharacter::TryRelease()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	Grabber->Release();
}

void ABaseCharacter::ReadyAbility()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (!bReadyAbility)
	{
		// 3인칭에서 1인칭으로 변경
		if (auto MainMesh = Cast<USkeletalMeshComponent>(GetMesh()->GetDefaultSubobjectByName(FName("SkeletalMesh"))))
		{
			MainMesh->SetVisibility(false);
		}

		//GetMesh()->SetVisibility(false);
		OriginTargetArmLength = CameraArm->TargetArmLength;
		CameraArm->TargetArmLength = 0.f;
	}

	bReadyAbility = true;
	TryRelease();
}

void ABaseCharacter::EndReadyAbility()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (bUsingAbility)
	{
		EndUseAbility();
	}

	bReadyAbility = false;

	if (auto MainMesh = Cast<USkeletalMeshComponent>(GetMesh()->GetDefaultSubobjectByName(FName("SkeletalMesh"))))
	{
		MainMesh->SetVisibility(true);
	}

	//GetMesh()->SetVisibility(true);
	CameraArm->TargetArmLength = OriginTargetArmLength;
}

void ABaseCharacter::UseAbility()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (!bReadyAbility)
	{
		return;
	}

	bUsingAbility = true;
	DrawingAbilityComponent->StartDrawing();
}

void ABaseCharacter::EndUseAbility()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		return;
	}

	if (!bReadyAbility)
	{
		return;
	}

	bUsingAbility = false;
	DrawingAbilityComponent->EndDrawing();

	// 먹 이펙트 정지
	if (AbilityEffectComponent->GetAsset())
	{
		AbilityEffectComponent->Deactivate();
	}
}

void ABaseCharacter::Action()
{
	if (DialogueSubsystem->IsDisplaying())
	{
		DialogueSubsystem->Next();
	}
	else if (LockCheckSubsystem->IsPlaying())
	{
		LockCheckSubsystem->Check();
	}
	else
	{
		if (!CurrentInteraction)
		{
			return;
		}

		if (IInteractable* Interaction = Cast<IInteractable>(CurrentInteraction))
		{
			Interaction->Interact();
		}
		else if (ICollectable* Collection = Cast<ICollectable>(CurrentInteraction))
		{
			SetCollectable(CurrentInteraction);
			Collection->Collect();
		}
	}
}

