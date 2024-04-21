// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interacts/Grabber.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "Abilities/DrawingAbilityComponent.h"

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

void ABaseCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddMovementInput(GetCameraForwardVector(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
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
	AddControllerYawInput(Value);
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::TryGrab()
{
	if (!bReadyAbility)
	{
		Grabber->Grab();
	}
}

void ABaseCharacter::TryRelease()
{
	Grabber->Release();
}

void ABaseCharacter::ReadyAbility()
{
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
	if (!bReadyAbility)
	{
		return;
	}

	bUsingAbility = true;
	DrawingAbilityComponent->StartDrawing();
}

void ABaseCharacter::EndUseAbility()
{
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

