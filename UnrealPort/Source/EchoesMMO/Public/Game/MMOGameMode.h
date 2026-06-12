#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Core/GameDataTypes.h"
#include "MMOGameMode.generated.h"

class AMMOCharacter;

UCLASS()
class ECHOESMMO_API AMMOGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMMOGameMode();

	virtual void BeginPlay() override;

	// ===== Zone Management =====
	UFUNCTION(BlueprintCallable, Category = "World")
	void LoadZone(EZone ZoneToLoad);

	UFUNCTION(BlueprintCallable, Category = "World")
	void UnloadZone(EZone ZoneToUnload);

	UFUNCTION(BlueprintCallable, Category = "World")
	FZoneData GetZoneData(EZone Zone) const;

	// ===== Spawning =====
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemyEncounter(EZone SpawnZone, int32 EnemyCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnNPC(EZone SpawnZone);

	// ===== Difficulty =====
	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetGameDifficulty(int32 NewDifficulty);

	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetGameDifficulty() const { return Difficulty; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TMap<EZone, FZoneData> ZoneDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TArray<FEnemyData> EnemyDatabase;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 Difficulty = 1;

	UPROPERTY()
	TArray<EZone> LoadedZones;

	void InitializeZoneDatabase();
	void InitializeEnemyDatabase();
};
