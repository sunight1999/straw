// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Abilities/LineDrawingComponent.h"

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

	LineDrawingComponent = CreateDefaultSubobject<ULineDrawingComponent>(TEXT("LineDrawing"));

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터를 움직이고 있는 방향으로 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	LineDrawingComponent->StartDrawing();
	LineDrawingComponent->AddPoint(GetActorLocation() + GetActorForwardVector() * 100.f);
	LineDrawingComponent->AddPoint(GetActorLocation() + GetActorForwardVector() * 200.f);
	LineDrawingComponent->AddPoint(GetActorLocation() + GetActorForwardVector() * 200.f + GetActorRightVector() * 100.f);
	LineDrawingComponent->EndDrawing();
	*/
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUsingAbility)
	{
		const FVector ControlForward = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
		LineDrawingComponent->AddPoint(GetActorLocation() + ControlForward * 200.f);

		//UE_LOG(LogTemp, Display, TEXT("ControlForward=%f.%f.%f"), ControlForward.X, ControlForward.Y, ControlForward.Z);
		//DrawDebugSphere(GetWorld(), GetActorLocation() + ControlForward * 200.f, 10, 16, FColor::Red, true);
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAction(FName("UseAbility"), IE_Pressed, this, &ABaseCharacter::UseAbility);
	PlayerInputComponent->BindAction(FName("UseAbility"), IE_Released, this, &ABaseCharacter::EndUseAbility);
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
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

void ABaseCharacter::UseAbility()
{
	bUsingAbility = true;
	LineDrawingComponent->StartDrawing();
}

void ABaseCharacter::EndUseAbility()
{
	bUsingAbility = false;
	LineDrawingComponent->EndDrawing();
}

