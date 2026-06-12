#include "Items/LootManager.h"
#include "Character/MMOCharacter.h"
#include "Enemies/EnemyCharacter.h"

ALootManager::ALootManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ALootManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALootManager::GenerateLootForEnemy(AEnemyCharacter* DefeatedEnemy, FVector DropLocation)
{
	if (!DefeatedEnemy)
	{
		return;
	}

	// 70% chance to drop common loot, 25% rare, 5% epic
	float RollValue = FMath::FRand();
	FInventoryItem DropItem;

	if (RollValue < 0.70f && CommonItems.Num() > 0)
	{
		DropItem = CommonItems[FMath::RandRange(0, CommonItems.Num() - 1)];
	}
	else if (RollValue < 0.95f && RareItems.Num() > 0)
	{
		DropItem = RareItems[FMath::RandRange(0, RareItems.Num() - 1)];
	}
	else if (EpicItems.Num() > 0)
	{
		DropItem = EpicItems[FMath::RandRange(0, EpicItems.Num() - 1)];
	}

	OnLootDropped.Broadcast(DropLocation, DropItem);
}

FInventoryItem ALootManager::GenerateRandomItem(int32 EnemyLevel)
{
	FInventoryItem Item;
	Item.Quantity = 1;
	Item.ItemName = FString::Printf(TEXT("Item_Level_%d"), EnemyLevel);
	Item.Description = TEXT("Loot from defeated enemy");
	Item.Price = 50 * EnemyLevel;

	if (EnemyLevel < 5)
	{
		Item.Rarity = TEXT("Common");
	}
	else if (EnemyLevel < 15)
	{
		Item.Rarity = TEXT("Uncommon");
	}
	else if (EnemyLevel < 30)
	{
		Item.Rarity = TEXT("Rare");
	}
	else if (EnemyLevel < 50)
	{
		Item.Rarity = TEXT("Epic");
	}
	else
	{
		Item.Rarity = TEXT("Legendary");
	}

	return Item;
}

TArray<FInventoryItem> ALootManager::GenerateLootTable(int32 Quantity, int32 EnemyLevel)
{
	TArray<FInventoryItem> LootTable;

	for (int32 i = 0; i < Quantity; ++i)
	{
		LootTable.Add(GenerateRandomItem(EnemyLevel));
	}

	return LootTable;
}

void ALootManager::PickupLoot(FInventoryItem& Item, AMMOCharacter* Collector)
{
	if (!Collector)
	{
		return;
	}

	Collector->AddItemToInventory(Item);
}

FString ALootManager::GetRarityColor(const FString& Rarity)
{
	if (Rarity == TEXT("Common"))
		return TEXT("#FFFFFF");
	if (Rarity == TEXT("Uncommon"))
		return TEXT("#00FF00");
	if (Rarity == TEXT("Rare"))
		return TEXT("#0070DD");
	if (Rarity == TEXT("Epic"))
		return TEXT("#A335EE");
	if (Rarity == TEXT("Legendary"))
		return TEXT("#FF8000");

	return TEXT("#FFFFFF");
}

int32 ALootManager::GetRarityWeight(const FString& Rarity)
{
	if (Rarity == TEXT("Common"))
		return 70;
	if (Rarity == TEXT("Uncommon"))
		return 20;
	if (Rarity == TEXT("Rare"))
		return 7;
	if (Rarity == TEXT("Epic"))
		return 2;
	if (Rarity == TEXT("Legendary"))
		return 1;

	return 0;
}
