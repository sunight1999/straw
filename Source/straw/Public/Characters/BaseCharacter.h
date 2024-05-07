// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGrabber;
class UDrawingAbilityComponent;
class UNiagaraComponent;

UCLASS()
class STRAW_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetCameraForwardVector(FRotator* OutCameraRotation = nullptr, bool bIncludePitch = false) const;

	void SetInteraction(AActor* Actor);
	void ReleaseInteraction();

protected:
	virtual void BeginPlay() override;

	/* Axis 바인딩 함수 */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	/* Action 바인딩 함수 */
	void TryGrab();
	void TryRelease();
	void ReadyAbility();
	void EndReadyAbility();
	void UseAbility();
	void EndUseAbility();
	void Action();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UGrabber* Grabber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* AbilityEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess="true"))
	UDrawingAbilityComponent* DrawingAbilityComponent;

	bool bReadyAbility = false;
	bool bUsingAbility = false;

	float OriginTargetArmLength;

	AActor* CurrentInteraction;
};
