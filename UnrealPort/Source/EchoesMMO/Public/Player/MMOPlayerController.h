#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MMOPlayerController.generated.h"

class AHUDS;
class AMMOCharacter;

UCLASS()
class ECHOESMMO_API AMMOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMMOPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ===== Character Creation =====
	UFUNCTION(BlueprintCallable, Category = "Character")
	void CreateCharacter(const FString& CharacterName, ECharacterClass SelectedClass, EFaction SelectedFaction);

	UFUNCTION(BlueprintCallable, Category = "Character")
	AMMOCharacter* GetPlayerCharacter() const;

	// ===== UI Management =====
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCharacterCreation();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCombatUI(class ACombatSystem* CombatSystem);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideCombatUI();

	// ===== Server RPC =====
	UFUNCTION(Server, Reliable, WithValidation, Category = "Server")
	void Server_CreateCharacter(const FString& CharacterName, ECharacterClass SelectedClass, EFaction SelectedFaction);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	AMMOCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class AHUDS> HUDClass;

	UPROPERTY()
	class AHUDS* GameHUD = nullptr;
};
