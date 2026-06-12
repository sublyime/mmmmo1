#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/GameDataTypes.h"
#include "InventoryWidget.generated.h"

class AMMOCharacter;

UCLASS()
class ECHOESMMO_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshInventory(AMMOCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnItemSelected(const FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UseItem(const FString& ItemName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void DropItem(const FString& ItemName);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* InventoryList = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SelectedItemDescription = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* UseItemButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DropItemButton = nullptr;

	FInventoryItem SelectedItem;
};
