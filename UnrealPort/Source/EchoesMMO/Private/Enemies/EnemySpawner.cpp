#include "Enemies/EnemySpawner.h"
#include "Enemies/EnemyCharacter.h"
#include "Game/MMOGameMode.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AMMOGameMode>(GetWorld()->GetAuthGameMode());
	TimeSinceLastSpawn = SpawnInterval;
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetOwnerRole().IsEquivalent(ROLE_Authority) || !bSpawnerActive)
	{
		return;
	}

	CleanupDeadEnemies();

	if (bAutoSpawn && ActiveEnemies.Num() < MaxActiveEnemies)
	{
		TimeSinceLastSpawn += DeltaTime;
		if (TimeSinceLastSpawn >= SpawnInterval)
		{
			SpawnRandomEnemy(AssignedZone);
			TimeSinceLastSpawn = 0.0f;
		}
	}
}

void AEnemySpawner::SpawnEnemy(const FEnemyData& EnemyData)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (!GameMode)
	{
		return;
	}

	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(SpawnLocation, SpawnRotation);
	if (NewEnemy)
	{
		NewEnemy->InitializeFromData(EnemyData);
		ActiveEnemies.Add(NewEnemy);
	}
}

void AEnemySpawner::SpawnRandomEnemy(EZone SpawnZone)
{
	if (!GameMode)
	{
		return;
	}

	// TODO: Get enemy database from game mode and pick random
	// For now, spawn a basic enemy
	FEnemyData BasicEnemy;
	BasicEnemy.EnemyName = TEXT("Ash Marauder");
	BasicEnemy.Level = 5;
	BasicEnemy.Stats.Strength = 14;
	BasicEnemy.ExperienceReward = 40;
	BasicEnemy.GoldReward = 25;

	SpawnEnemy(BasicEnemy);
}

void AEnemySpawner::EnableSpawner()
{
	bSpawnerActive = true;
}

void AEnemySpawner::DisableSpawner()
{
	bSpawnerActive = false;
}

void AEnemySpawner::CleanupDeadEnemies()
{
	ActiveEnemies.RemoveAll([](AEnemyCharacter* Enemy)
		{
			return Enemy == nullptr || Enemy->GetCurrentHP() <= 0;
		});
}

FVector AEnemySpawner::GetRandomSpawnLocation() const
{
	float RandomAngle = FMath::FRand() * 360.0f;
	float RandomDistance = FMath::FRand() * SpawnRadius;
	float X = FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance;
	float Y = FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance;

	return GetActorLocation() + FVector(X, Y, 0.0f);
}
