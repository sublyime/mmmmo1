#include "Player/MMOPlayerController.h"
#include "Character/MMOCharacter.h"
#include "Combat/CombatSystem.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

AMMOPlayerController::AMMOPlayerController()
{
	bReplicates = true;
}

void AMMOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		ShowMainMenu();
	}
}

void AMMOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMMOPlayerController::CreateCharacter(const FString& CharacterName, ECharacterClass SelectedClass, EFaction SelectedFaction)
{
	Server_CreateCharacter(CharacterName, SelectedClass, SelectedFaction);
}

AMMOCharacter* AMMOPlayerController::GetPlayerCharacter() const
{
	return PlayerCharacter;
}

void AMMOPlayerController::ShowMainMenu()
{
	// TODO: Load and show main menu widget
}

void AMMOPlayerController::ShowCharacterCreation()
{
	// TODO: Load and show character creation widget
}

void AMMOPlayerController::ShowGameHUD()
{
	// TODO: Load and show main game HUD
}

void AMMOPlayerController::ShowCombatUI(ACombatSystem* CombatSystem)
{
	// TODO: Load and show combat UI widget
}

void AMMOPlayerController::HideCombatUI()
{
	// TODO: Hide combat UI widget
}

void AMMOPlayerController::Server_CreateCharacter_Implementation(const FString& CharacterName, ECharacterClass SelectedClass, EFaction SelectedFaction)
{
	// Spawn player character
	AMMOCharacter* NewCharacter = GetWorld()->SpawnActor<AMMOCharacter>();
	if (NewCharacter)
	{
		NewCharacter->CharacterName = CharacterName;
		NewCharacter->CharacterClass = SelectedClass;
		NewCharacter->Faction = SelectedFaction;

		// Possess the character
		Possess(NewCharacter);
		PlayerCharacter = NewCharacter;

		// Show game HUD
		ShowGameHUD();
	}
}

bool AMMOPlayerController::Server_CreateCharacter_Validate(const FString& CharacterName, ECharacterClass SelectedClass, EFaction SelectedFaction)
{
	return !CharacterName.IsEmpty();
}
