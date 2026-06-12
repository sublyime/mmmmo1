#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMOHUD.generated.h"

class UMainHUD;
class AMMOCharacter;

UCLASS()
class ECHOESMMO_API AMMOHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMMOHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowMainHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowCharacterCreation();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowCombatOverlay();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideCombatOverlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UMainHUD> MainHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UMainHUD* MainHUD = nullptr;

private:
	FVector2D ScreenCenter;
};
