// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
		DrawingAbilityComponent->AddPoint();
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);

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

void ABaseCharacter::ReadyAbility()
{
	if (!bReadyAbility)
	{
		GetMesh()->SetVisibility(false);
		OriginTargetArmLength = CameraArm->TargetArmLength;
		CameraArm->TargetArmLength = 0.f;
	}

	bReadyAbility = true;
}

void ABaseCharacter::EndReadyAbility()
{
	if (bUsingAbility)
	{
		EndUseAbility();
	}

	bReadyAbility = false;

	GetMesh()->SetVisibility(true);
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
}

