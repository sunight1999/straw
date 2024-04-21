// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDrawingAbilityComponent;

UCLASS()
class STRAW_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetCameraForwardVector(FRotator* OutCameraRotation = nullptr, bool bIncludePitch = false) const;

protected:
	virtual void BeginPlay() override;

	/* 캐릭터 이동 관련 */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	/* 능력 사용 관련 */
	void ReadyAbility();
	void EndReadyAbility();
	void UseAbility();
	void EndUseAbility();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess="true"))
	UDrawingAbilityComponent* DrawingAbilityComponent;

	bool bReadyAbility = false;
	bool bUsingAbility = false;

	float OriginTargetArmLength;
};
