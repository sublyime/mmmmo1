#include "UI/WorldMapWidget.h"
#include "Components/Canvas.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UWorldMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TravelButton)
	{
		TravelButton->OnClicked.AddDynamic(this, &UWorldMapWidget::TravelToZone);
	}
}

void UWorldMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update map rendering
	RefreshMapDisplay();
}

void UWorldMapWidget::RefreshMapDisplay()
{
	// TODO: Iterate through available zones in the GameMode/GameState 
	// and draw them onto the MapCanvas. Highlight the zone matching PlayerCharacter->CurrentZone.
	// Example:
	// if (MapCanvas) { /* Draw Map Logic */ }
}

void UWorldMapWidget::OnZoneClicked(int32 ZoneIndex)
{
	SelectedZoneIndex = ZoneIndex;

	if (ZoneInfoText)
	{
		// Ideally, fetch the Zone Name from your GameDataTypes instead of just the index
		FString ZoneName = FString::Printf(TEXT("Zone %d"), ZoneIndex);
		ZoneInfoText->SetText(FText::FromString(FString::Printf(TEXT("Selected: %s\nClick Travel to enter."), *ZoneName)));
	}
}

void UWorldMapWidget::TravelToZone()
{
	if (SelectedZoneIndex < 0)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMMOCharacter* PlayerCharacter = Cast<AMMOCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			// Assuming AMMOCharacter or PlayerController has a Server RPC to handle travel
			// Example: PlayerCharacter->Server_TravelToZone(static_cast<EZone>(SelectedZoneIndex));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Traveling to new zone..."));
		}
	}
}
