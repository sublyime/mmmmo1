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
	// TODO: Draw map zones and current player position
}

void UWorldMapWidget::OnZoneClicked(int32 ZoneIndex)
{
	SelectedZoneIndex = ZoneIndex;

	// TODO: Update zone info display
	if (ZoneInfoText)
	{
		ZoneInfoText->SetText(FText::FromString(FString::Printf(TEXT("Zone %d selected"), ZoneIndex)));
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
			// TODO: Travel to selected zone
		}
	}
}
