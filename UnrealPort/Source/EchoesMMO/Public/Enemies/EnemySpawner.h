#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/GameDataTypes.h"
#include "EnemySpawner.generated.h"

class AMMOCharacter;

UCLASS()
class ECHOESMMO_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ===== Spawning =====
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy(const FEnemyData& EnemyData);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRandomEnemy(EZone SpawnZone);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void EnableSpawner();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void DisableSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	EZone AssignedZone = EZone::IronhavenCity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxActiveEnemies = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bAutoSpawn = true;

protected:
	UPROPERTY()
	TArray<class AEnemyCharacter*> ActiveEnemies;

	UPROPERTY()
	float TimeSinceLastSpawn = 0.0f;

	UPROPERTY()
	bool bSpawnerActive = true;

	UPROPERTY()
	class AMMOGameMode* GameMode = nullptr;

	void CleanupDeadEnemies();
	FVector GetRandomSpawnLocation() const;
};
