#include "UI/MainHUD.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Animation/UMGSequencePlayer.h"
#include "Character/MMOCharacter.h"
#include "Combat/CombatSystem.h"
#include "Player/MMOPlayerController.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button events
	if (EnterRealmButton)
	{
		EnterRealmButton->OnClicked.AddDynamic(this, &UMainHUD::OnEnterRealmClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainHUD::OnSettingsClicked);
	}

	if (CreateCharacterButton)
	{
		CreateCharacterButton->OnClicked.AddDynamic(this, &UMainHUD::OnCreateCharacterClicked);
	}

	if (FleeButton)
	{
		FleeButton->OnClicked.AddDynamic(this, &UMainHUD::OnFleeClicked);
	}

	// Initialize with title screen
	ShowTitleScreen();
}

void UMainHUD::NativeDestruct()
{
	Super::NativeDestruct();

	// Unbind button events
	if (EnterRealmButton)
	{
		EnterRealmButton->OnClicked.RemoveDynamic(this, &UMainHUD::OnEnterRealmClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.RemoveDynamic(this, &UMainHUD::OnSettingsClicked);
	}

	if (CreateCharacterButton)
	{
		CreateCharacterButton->OnClicked.RemoveDynamic(this, &UMainHUD::OnCreateCharacterClicked);
	}

	if (FleeButton)
	{
		FleeButton->OnClicked.RemoveDynamic(this, &UMainHUD::OnFleeClicked);
	}
}

void UMainHUD::ShowTitleScreen()
{
	if (TitleScreen)
	{
		TitleScreen->SetVisibility(ESlateVisibility::Visible);
	}
	if (CharacterCreationScreen)
	{
		CharacterCreationScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameHUD)
	{
		GameHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUD::ShowCharacterCreation()
{
	if (TitleScreen)
	{
		TitleScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CharacterCreationScreen)
	{
		CharacterCreationScreen->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUD::ShowGameScreen()
{
	if (TitleScreen)
	{
		TitleScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CharacterCreationScreen)
	{
		CharacterCreationScreen->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameHUD)
	{
		GameHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUD::ShowCombatOverlay()
{
	if (CombatOverlay)
	{
		CombatOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUD::HideCombatOverlay()
{
	if (CombatOverlay)
	{
		CombatOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUD::SelectCharacterClass(ECharacterClass SelectedClass)
{
	SelectedCharacterClass = SelectedClass;
	// TODO: Update UI to highlight selected class
}

void UMainHUD::SelectFaction(EFaction SelectedFaction)
{
	SelectedCharacterFaction = SelectedFaction;
	// TODO: Update UI to highlight selected faction
}

void UMainHUD::BeginCharacterCreation(const FString& CharacterName)
{
	if (CharacterName.IsEmpty())
	{
		ShowNotification(TEXT("Please enter a character name!"), FLinearColor::Red);
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMMOPlayerController* MMOController = Cast<AMMOPlayerController>(PlayerController);
		if (MMOController)
		{
			MMOController->CreateCharacter(CharacterName, SelectedCharacterClass, SelectedCharacterFaction);
			ShowGameScreen();
		}
	}
}

void UMainHUD::UpdatePlayerHUD(AMMOCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	// Update player info
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(Player->CharacterName));
	}

	if (PlayerLevelText)
	{
		PlayerLevelText->SetText(FText::AsNumber(Player->Progression.Level));
	}

	// Update bars
	float HPPercent = Player->GetHealthPercent();
	if (HPBar)
	{
		HPBar->SetPercent(HPPercent);
	}
	if (HPText)
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Player->CurrentHP, Player->MaxHP)));
	}

	float MPPercent = Player->GetManaPercent();
	if (MPBar)
	{
		MPBar->SetPercent(MPPercent);
	}
	if (MPText)
	{
		MPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Player->CurrentMP, Player->MaxMP)));
	}

	float XPPercent = (float)Player->Progression.CurrentExperience / Player->Progression.ExperienceToNextLevel;
	if (XPBar)
	{
		XPBar->SetPercent(FMath::Min(1.0f, XPPercent));
	}

	// Update stats
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(Player->Progression.Gold));
	}

	if (KillsText)
	{
		KillsText->SetText(FText::AsNumber(Player->Progression.TotalKills));
	}

	if (RatingText)
	{
		RatingText->SetText(FText::AsNumber(Player->Progression.Rating));
	}

	// Note: Zone name retrieval would require access to zone database
	// For now, just set a placeholder
	if (ZoneText)
	{
		ZoneText->SetText(FText::FromString(TEXT("Current Zone")));
	}
}

void UMainHUD::UpdateCombatUI(ACombatSystem* CombatSystem)
{
	if (!CombatSystem)
	{
		return;
	}

	// TODO: Update combat UI elements
	if (RoundDisplayText)
	{
		RoundDisplayText->SetText(FText::FromString(FString::Printf(TEXT("Round %d"), CombatSystem->GetCurrentRound())));
	}
}

void UMainHUD::AddCombatLog(const FString& Message)
{
	if (!CombatLogBox)
	{
		return;
	}

	UTextBlock* LogEntry = NewObject<UTextBlock>(CombatLogBox);
	LogEntry->SetText(FText::FromString(Message));
	LogEntry->SetColorAndOpacity(FLinearColor::White);
	CombatLogBox->AddChild(LogEntry);

	// Auto-scroll to bottom
	CombatLogBox->ScrollToEnd();

	// Remove old entries if too many
	if (CombatLogBox->GetChildrenCount() > 20)
	{
		CombatLogBox->RemoveChildAt(0);
	}
}

void UMainHUD::ShowNotification(const FString& Message, FLinearColor Color)
{
	if (!NotificationContainer)
	{
		return;
	}

	UTextBlock* Notification = NewObject<UTextBlock>(NotificationContainer);
	Notification->SetText(FText::FromString(Message));
	Notification->SetColorAndOpacity(Color);
	NotificationContainer->AddChild(Notification);

	// Auto-remove after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(
		FTimerHandle(),
		[this, Notification]()
		{
			if (Notification && NotificationContainer)
			{
				NotificationContainer->RemoveChild(Notification);
			}
		},
		5.0f,
		false
	);
}

void UMainHUD::ShowLevelUp(int32 NewLevel)
{
	ShowNotification(FString::Printf(TEXT("🎉 Level Up! You are now level %d"), NewLevel), FLinearColor::Yellow);
}

void UMainHUD::OnEnterRealmClicked()
{
	ShowCharacterCreation();
}

void UMainHUD::OnSettingsClicked()
{
	ShowNotification(TEXT("Settings panel coming soon!"), FLinearColor::Cyan);
}

void UMainHUD::OnCreateCharacterClicked()
{
	if (CharacterNameInput)
	{
		BeginCharacterCreation(CharacterNameInput->GetText().ToString());
	}
}

void UMainHUD::OnFleeClicked()
{
	HideCombatOverlay();
	ShowNotification(TEXT("You fled combat!"), FLinearColor::Yellow);
}
