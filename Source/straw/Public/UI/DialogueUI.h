// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUI.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class STRAW_API UDialogueUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Type(FString Name, FString Line);
	void Type();
	void TypeAtOnce();

	bool IsTyping() const { return bIsTyping; }

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* DialoguePanel;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;

	UPROPERTY(EditAnywhere)
	float TypeSpeed = 0.1f;

	FTimerHandle TypeTimer;
	FString CurrentLine;
	int CurrentLineCaret = 0;
	bool bIsTyping = false;
};
