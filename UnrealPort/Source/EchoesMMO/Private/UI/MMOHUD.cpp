#include "UI/MMOHUD.h"
#include "UI/MainHUD.h"
#include "Blueprint/UserWidget.h"

AMMOHUD::AMMOHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMMOHUD::BeginPlay()
{
	Super::BeginPlay();

	ScreenCenter = FVector2D(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	if (MainHUDClass)
	{
		MainHUD = CreateWidget<UMainHUD>(GetOwningPlayerController(), MainHUDClass);
		if (MainHUD)
		{
			MainHUD->AddToViewport(0);
		}
	}
}

void AMMOHUD::DrawHUD()
{
	Super::DrawHUD();

	// Custom debug drawing can go here if needed
}

void AMMOHUD::ShowMainHUD()
{
	if (MainHUD)
	{
		MainHUD->ShowGameScreen();
	}
}

void AMMOHUD::ShowCharacterCreation()
{
	if (MainHUD)
	{
		MainHUD->ShowCharacterCreation();
	}
}

void AMMOHUD::ShowCombatOverlay()
{
	if (MainHUD)
	{
		MainHUD->ShowCombatOverlay();
	}
}

void AMMOHUD::HideCombatOverlay()
{
	if (MainHUD)
	{
		MainHUD->HideCombatOverlay();
	}
}
