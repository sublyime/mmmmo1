#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/GameDataTypes.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MainHUD.generated.h"

class AMMOCharacter;
class ACombatSystem;

UCLASS()
class ECHOESMMO_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// ===== Screen Management =====
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowTitleScreen();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCharacterCreation();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameScreen();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCombatOverlay();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideCombatOverlay();

	// ===== Character Creation =====
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectCharacterClass(ECharacterClass SelectedClass);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectFaction(EFaction SelectedFaction);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void BeginCharacterCreation(const FString& CharacterName);

	// ===== HUD Updates =====
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdatePlayerHUD(AMMOCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateCombatUI(ACombatSystem* CombatSystem);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddCombatLog(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowNotification(const FString& Message, FLinearColor Color = FLinearColor::White);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowLevelUp(int32 NewLevel);

protected:
	// Title screen widgets
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* TitleScreen = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* EnterRealmButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SettingsButton = nullptr;

	// Character creation widgets
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CharacterCreationScreen = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* ClassSelectionGrid = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* FactionSelectionGrid = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* CharacterNameInput = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CreateCharacterButton = nullptr;

	// Game HUD widgets
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* GameHUD = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerNameText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerLevelText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HPBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* MPBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* XPBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HPText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MPText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GoldText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KillsText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RatingText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ZoneText = nullptr;

	// Combat UI
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CombatOverlay = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EnemyNameText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* EnemyHPBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RoundDisplayText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* CombatLogBox = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* SkillButtonContainer = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* FleeButton = nullptr;

	// Notifications
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* NotificationContainer = nullptr;

	// Data tracking
	ECharacterClass SelectedCharacterClass = ECharacterClass::Warrior;
	EFaction SelectedCharacterFaction = EFaction::IronveilDominion;

	// Button callbacks
	void OnEnterRealmClicked();
	void OnSettingsClicked();
	void OnCreateCharacterClicked();
	void OnFleeClicked();
};
