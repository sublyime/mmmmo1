#include "Game/MMOGameMode.h"
#include "Character/MMOCharacter.h"
#include "Player/MMOPlayerController.h"

AMMOGameMode::AMMOGameMode()
{
	PlayerControllerClass = AMMOPlayerController::StaticClass();
	DefaultPawnClass = AMMOCharacter::StaticClass();
}

void AMMOGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeZoneDatabase();
	InitializeEnemyDatabase();

	// Load starting zone
	LoadZone(EZone::IronhavenCity);
}

void AMMOGameMode::InitializeZoneDatabase()
{
	// Ironhaven City
	FZoneData IronhavenData;
	IronhavenData.ZoneName = TEXT("Ironhaven City");
	IronhavenData.ZoneType = EZone::IronhavenCity;
	IronhavenData.SpawnLocation = FVector(0, 0, 100);
	IronhavenData.MinLevel = 1;
	IronhavenData.MaxLevel = 99;
	IronhavenData.PvPStatus = EPvPStatus::SafeZone;
	IronhavenData.Description = TEXT("The capital city. Trade, guilds, and politics.");
	IronhavenData.bCanRespawnHere = true;
	ZoneDatabase.Add(EZone::IronhavenCity, IronhavenData);

	// Ashen Wastes
	FZoneData AshenWastesData;
	AshenWastesData.ZoneName = TEXT("Ashen Wastes");
	AshenWastesData.ZoneType = EZone::AshenWastes;
	AshenWastesData.SpawnLocation = FVector(2000, 0, 100);
	AshenWastesData.MinLevel = 1;
	AshenWastesData.MaxLevel = 15;
	AshenWastesData.PvPStatus = EPvPStatus::PvPZone;
	AshenWastesData.Description = TEXT("Contested territory. Outlaws and bounties thrive here.");
	AshenWastesData.bCanRespawnHere = false;
	ZoneDatabase.Add(EZone::AshenWastes, AshenWastesData);

	// Silvermere Forest
	FZoneData SilvermereData;
	SilvermereData.ZoneName = TEXT("Silvermere Forest");
	SilvermereData.ZoneType = EZone::SilvermereForest;
	SilvermereData.SpawnLocation = FVector(4000, 0, 100);
	SilvermereData.MinLevel = 5;
	SilvermereData.MaxLevel = 20;
	SilvermereData.PvPStatus = EPvPStatus::SafeZone;
	SilvermereData.Description = TEXT("Ancient forest teeming with Arcane beasts.");
	SilvermereData.bCanRespawnHere = true;
	ZoneDatabase.Add(EZone::SilvermereForest, SilvermereData);

	// Add more zones as needed...
}

void AMMOGameMode::InitializeEnemyDatabase()
{
	// Ash Marauder
	FEnemyData MarauderData;
	MarauderData.EnemyName = TEXT("Ash Marauder");
	MarauderData.Level = 5;
	MarauderData.Stats.Strength = 14;
	MarauderData.Stats.Vitality = 12;
	MarauderData.ExperienceReward = 40;
	MarauderData.GoldReward = 25;
	MarauderData.EnemyType = TEXT("pvp");
	EnemyDatabase.Add(MarauderData);

	// Shadow Wraith
	FEnemyData WraithData;
	WraithData.EnemyName = TEXT("Shadow Wraith");
	WraithData.Level = 4;
	WraithData.Stats.Intelligence = 16;
	WraithData.Stats.Agility = 14;
	WraithData.ExperienceReward = 35;
	WraithData.GoldReward = 20;
	WraithData.EnemyType = TEXT("magic");
	EnemyDatabase.Add(WraithData);

	// Add more enemies as needed...
}

void AMMOGameMode::LoadZone(EZone ZoneToLoad)
{
	if (!LoadedZones.Contains(ZoneToLoad))
	{
		LoadedZones.Add(ZoneToLoad);
		// TODO: Load zone assets, spawn NPCs, monsters, etc.
	}
}

void AMMOGameMode::UnloadZone(EZone ZoneToUnload)
{
	LoadedZones.Remove(ZoneToUnload);
	// TODO: Unload zone assets, clean up actors, etc.
}

FZoneData AMMOGameMode::GetZoneData(EZone Zone) const
{
	if (const FZoneData* Data = ZoneDatabase.Find(Zone))
	{
		return *Data;
	}
	return FZoneData();
}

void AMMOGameMode::SpawnEnemyEncounter(EZone SpawnZone, int32 EnemyCount)
{
	if (EnemyDatabase.Num() == 0)
	{
		return;
	}

	FZoneData ZoneData = GetZoneData(SpawnZone);
	for (int32 i = 0; i < EnemyCount; ++i)
	{
		FEnemyData EnemyData = EnemyDatabase[FMath::RandRange(0, EnemyDatabase.Num() - 1)];
		// TODO: Spawn enemy actor based on EnemyData
	}
}

void AMMOGameMode::SpawnNPC(EZone SpawnZone)
{
	// TODO: Spawn NPC actor in zone
}

void AMMOGameMode::SetGameDifficulty(int32 NewDifficulty)
{
	Difficulty = FMath::Clamp(NewDifficulty, 1, 10);
}
