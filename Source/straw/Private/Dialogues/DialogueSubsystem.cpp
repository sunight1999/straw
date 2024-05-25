// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogues/DialogueSubsystem.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "HUD/MainHUD.h"
#include "UI/DialogueUI.h"

UDialogueSubsystem::UDialogueSubsystem()
{
	ConstructorHelpers::FObjectFinder<UDataTable> DialogueDataTableFinder(TEXT("/Game/Datas/DT_Dialogues"));
	if (DialogueDataTableFinder.Succeeded())
	{
		DialogueDataTable = DialogueDataTableFinder.Object;
	}
}

FDialogue* UDialogueSubsystem::StartDialogue(FName DialogueID)
{
	if (bIsDisplaying)
	{
		return nullptr;
	}

	if (!DialogueWidget)
	{
		FetchUI();
	}

	bIsDisplaying = true;
	DialogueWidget->SetVisibility(ESlateVisibility::Visible);

	CurrentDialogue = DialogueDataTable->FindRow<FDialogue>(DialogueID, FString(""));
	if (!CurrentDialogue)
	{
		return nullptr;
	}

	Next();

	return CurrentDialogue;
}

void UDialogueSubsystem::EndDialogue()
{
	bIsDisplaying = false;
	DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
	CurrentDialogue = nullptr;
	CurrentLineIndex = 0;

	if (CurrentDialogueDelegate.IsBound())
	{
		CurrentDialogueDelegate.Execute();
	}
}

void UDialogueSubsystem::Next()
{
	if (DialogueWidget->IsTyping() || !CurrentDialogue)
	{
		return;
	}

	if (CurrentLineIndex < CurrentDialogue->Lines.Num())
	{
		DialogueWidget->Type(CurrentDialogue->Lines[CurrentLineIndex++].Line);
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueSubsystem::FetchUI()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
	if (!MainHUD)
	{
		return;
	}

	DialogueWidget = MainHUD->GetDialogueUI();
}
