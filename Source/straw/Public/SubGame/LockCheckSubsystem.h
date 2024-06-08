// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LockCheckSubsystem.generated.h"

class ULockCheckUI;

using FLockCheckEndDelegate = TDelegate<void(bool)>;

/**
 * 
 */
UCLASS()
class STRAW_API ULockCheckSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	template<class UserClass>
	void StartLockCheck(UserClass* Object, typename FLockCheckEndDelegate::TMethodPtr<UserClass> DialogueCallback);
	void Check();
	bool IsPlaying() const { return bIsPlaying; }

private:
	void FetchUI();
	void StartLockCheck();
	void Next();
	void End(bool bResult);

	ULockCheckUI* LockCheckUI;

	bool bIsPlaying = false;

	int CurrentLevel = 0;
	int MaxLevel = 3;
	float StartSpeed = 150.f;
	float SpeedStep = 50.f;
	float StartCheckBarWidth = .1f;
	float CheckBarWidthStep = -.01f;

	// 해제 성공 또는 실패 시 호출, 성공 여부를 파라미터로 전달
	FLockCheckEndDelegate LockCheckEndDelegate;
};

template<class UserClass>
inline void ULockCheckSubsystem::StartLockCheck(UserClass* Object, typename FLockCheckEndDelegate::TMethodPtr<UserClass> DialogueCallback)
{
	if (bIsPlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 자물쇠 풀기를 진행 중입니다."));
		return;
	}

	LockCheckEndDelegate.Unbind();
	LockCheckEndDelegate.BindUObject(Object, DialogueCallback);

	StartLockCheck();
}
