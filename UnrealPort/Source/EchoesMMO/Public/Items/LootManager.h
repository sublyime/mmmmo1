#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/GameDataTypes.h"
#include "LootManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_Two(FLootDropped, FVector, Location, FInventoryItem, Item);

UCLASS()
class ECHOESMMO_API ALootManager : public AActor
{
	GENERATED_BODY()

public:
	ALootManager();

	virtual void Tick(float DeltaTime) override;

	// ===== Loot Generation =====
	UFUNCTION(BlueprintCallable, Category = "Loot")
	void GenerateLootForEnemy(class AEnemyCharacter* DefeatedEnemy, FVector DropLocation);

	UFUNCTION(BlueprintCallable, Category = "Loot")
	FInventoryItem GenerateRandomItem(int32 EnemyLevel);

	UFUNCTION(BlueprintCallable, Category = "Loot")
	TArray<FInventoryItem> GenerateLootTable(int32 Quantity, int32 EnemyLevel);

	// ===== Loot Pickup =====
	UFUNCTION(BlueprintCallable, Category = "Loot")
	void PickupLoot(FInventoryItem& Item, class AMMOCharacter* Collector);

	UPROPERTY(BlueprintAssignable, Category = "Loot")
	FLootDropped OnLootDropped;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float LootPickupRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float LootSpawnHeight = 100.0f;

	// Loot tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FInventoryItem> CommonItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FInventoryItem> RareItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FInventoryItem> EpicItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FInventoryItem> LegendaryItems;

	FString GetRarityColor(const FString& Rarity);
	int32 GetRarityWeight(const FString& Rarity);
};
