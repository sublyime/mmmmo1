#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMapWidget.generated.h"

class AMMOCharacter;

UCLASS()
class ECHOESMMO_API UWorldMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshMapDisplay();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnZoneClicked(int32 ZoneIndex);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TravelToZone();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvas* MapCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ZoneInfoText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TravelButton = nullptr;

	int32 SelectedZoneIndex = -1;
};
