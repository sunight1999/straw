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
class ATraditionalKey;
class ATraditionalOrnament;
class UDialogueSubsystem;
class ULockCheckSubsystem;
class AMainHUD;
class UOrnamentOverlay;

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
	void SetCollectable(AActor* Actor);
	void SetRootable(AActor* Actor);

	/* 퀘스트 관련 함수 */
	ATraditionalKey* HasTraditionalKey(FString KeyID);
	float GetTraditionalKeyOffset(FString KeyID);
	void UseTraditionalKey(FString KeyID);
	bool IsTraditionalOrnamentReady();

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
	/* 조작 및 카메라*/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UGrabber* Grabber;

	float OriginTargetArmLength;

	/* 능력 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* AbilityEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* AbilityReadyEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UDrawingAbilityComponent* DrawingAbilityComponent;

	bool bReadyAbility = false;
	bool bUsingAbility = false;

	/* 상호작용 */
	AActor* CurrentInteraction;

	/* 퀘스트 */
	TArray<ATraditionalKey*> TraditionalKeys;
	TMap<FString, float> TraditionalKeyOffsetMap;

	/* Rootable */
	bool TraditionalOrnaments[4];

	/* 서브시스템 */
	UDialogueSubsystem* DialogueSubsystem;
	ULockCheckSubsystem* LockCheckSubsystem;

	/* UI */
	AMainHUD* MainHUD;
	UOrnamentOverlay* OrnamentOverlay;
};
