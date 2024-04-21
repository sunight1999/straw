// Fill out your copyright notice in the Description page of Project Settings.


#include "Interacts/Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Characters/BaseCharacter.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ABaseCharacter>(GetOwner());
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && Owner && PhysicsHandle->GetGrabbedComponent())
	{
		FRotator CameraRotation;
		FVector CameraForward;
		CameraForward = Owner->GetCameraForwardVector(&CameraRotation, true);

		FVector TargetLocation = GetComponentLocation() + CameraForward * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, CameraRotation);
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult;

	if (GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();

		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add(FName("Grabbed"));
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle)
		return;

	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent)
	{
		GrabbedComponent->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	if (!PhysicsHandle || !Owner)
		return false;

	FVector Start = GetComponentLocation();
	FVector End = Start + Owner->GetCameraForwardVector(nullptr, true) * MaxGrabDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);

	if (HasHit)
		OutHitResult = HitResult;

	return HasHit;
}

