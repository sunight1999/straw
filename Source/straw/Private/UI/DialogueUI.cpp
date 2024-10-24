// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueUI.h"
#include "Components/TextBlock.h"

void UDialogueUI::Type(FString Name, FString Line)
{
	CurrentLine = Line;
	CurrentLineCaret = 0;
	bIsTyping = true;

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TypeTimer, this, &UDialogueUI::Type, TypeSpeed, true, 0.f);

	NameText->SetText(FText::FromString(Name));
	DialogueText->SetText(FText::GetEmpty());
}

void UDialogueUI::Type()
{
	if (CurrentLineCaret <= CurrentLine.Len())
	{
		DialogueText->SetText(FText::FromString(CurrentLine.Left(CurrentLineCaret++)));
	}
	else
	{
		UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(TypeTimer);

		bIsTyping = false;
	}
}

void UDialogueUI::TypeAtOnce()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(TypeTimer);

	DialogueText->SetText(FText::FromString(CurrentLine));
	bIsTyping = false;
}
