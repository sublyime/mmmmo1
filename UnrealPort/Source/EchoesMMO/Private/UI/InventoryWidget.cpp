#include "UI/InventoryWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/MMOCharacter.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UseItemButton)
	{
		UseItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::UseItem);
	}

	if (DropItemButton)
	{
		DropItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::DropItem);
	}
}

void UInventoryWidget::RefreshInventory(AMMOCharacter* Player)
{
	if (!Player || !InventoryList)
	{
		return;
	}

	InventoryList->ClearChildren();

	for (const FInventoryItem& Item : Player->Inventory)
	{
		UTextBlock* ItemEntry = NewObject<UTextBlock>(InventoryList);
		ItemEntry->SetText(FText::FromString(FString::Printf(TEXT("%s x%d"), *Item.ItemName, Item.Quantity)));
		InventoryList->AddChild(ItemEntry);
	}
}

void UInventoryWidget::OnItemSelected(const FInventoryItem& Item)
{
	SelectedItem = Item;

	if (SelectedItemDescription)
	{
		SelectedItemDescription->SetText(FText::FromString(Item.Description));
	}
}

void UInventoryWidget::UseItem(const FString& ItemName)
{
	if (ItemName.IsEmpty() || ItemName == SelectedItem.ItemName)
	{
		return;
	}

	// TODO: Apply item effects
}

void UInventoryWidget::DropItem(const FString& ItemName)
{
	if (ItemName.IsEmpty())
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		AMMOCharacter* PlayerCharacter = Cast<AMMOCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->RemoveItemFromInventory(ItemName, 1);
			RefreshInventory(PlayerCharacter);
		}
	}
}
