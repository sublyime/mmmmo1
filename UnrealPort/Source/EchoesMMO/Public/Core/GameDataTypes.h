#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameDataTypes.generated.h"

/** Character Classes */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior UMETA(DisplayName = "Warbringer"),
	Mage UMETA(DisplayName = "Arcanist"),
	Rogue UMETA(DisplayName = "Shadowblade"),
	Hunter UMETA(DisplayName = "Voidhunter"),
};

/** Player Factions */
UENUM(BlueprintType)
enum class EFaction : uint8
{
	IronveilDominion UMETA(DisplayName = "Ironveil Dominion"),
	TheSunkenCourt UMETA(DisplayName = "The Sunken Court"),
	VoidbornConclave UMETA(DisplayName = "Voidborn Conclave"),
};

/** Zones/Regions */
UENUM(BlueprintType)
enum class EZone : uint8
{
	IronhavenCity UMETA(DisplayName = "Ironhaven City"),
	AshenWastes UMETA(DisplayName = "Ashen Wastes"),
	SilvermereForest UMETA(DisplayName = "Silvermere Forest"),
	VoidRift UMETA(DisplayName = "The Void Rift"),
	GlacialPeaks UMETA(DisplayName = "Glacial Peaks"),
	SunkenTemple UMETA(DisplayName = "Sunken Temple"),
	WarFront UMETA(DisplayName = "War Front"),
};

/** PvP Status */
UENUM(BlueprintType)
enum class EPvPStatus : uint8
{
	SafeZone UMETA(DisplayName = "Safe Zone"),
	PvPZone UMETA(DisplayName = "PvP Zone"),
	ElitePvP UMETA(DisplayName = "Elite PvP"),
};

/** Combat Effect Types */
UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Magic UMETA(DisplayName = "Magic"),
	Poison UMETA(DisplayName = "Poison"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff"),
};

/** Damage Type */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Magic UMETA(DisplayName = "Magic"),
	Poison UMETA(DisplayName = "Poison"),
};

/** Character Stats Structure */
USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Strength = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Intelligence = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Vitality = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Agility = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Dexterity = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Wisdom = 10;

	// Calculated stats
	int32 GetHealthPoints() const;
	int32 GetManaPoints() const;
	int32 GetAttack() const;
	int32 GetDefense() const;
	int32 GetSpeed() const;
};

/** Combat Effect */
USTRUCT(BlueprintType)
struct FCombatEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EffectName = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RemainingTurns = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEffectType EffectType = EEffectType::Buff;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Magnitude = 0.0f;
};

/** Skill/Ability Data */
USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SkillName = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ManaCost = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cooldown = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDamageType DamageType = EDamageType::Physical;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinDamage = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxDamage = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECharacterClass RequiredClass = ECharacterClass::Warrior;
};

/** Zone/Region Data */
USTRUCT(BlueprintType)
struct FZoneData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ZoneName = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EZone ZoneType = EZone::IronhavenCity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinLevel = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxLevel = 99;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPvPStatus PvPStatus = EPvPStatus::SafeZone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanRespawnHere = true;
};

/** Enemy/NPC Data */
USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EnemyName = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Level = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCharacterStats Stats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ExperienceReward = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 GoldReward = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EnemyType = FString();
};

/** Player Progression Data */
USTRUCT(BlueprintType)
struct FPlayerProgression
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Level = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 CurrentExperience = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 ExperienceToNextLevel = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalKills = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalDeaths = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Rating = 1200;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Gold = 1250;
};

/** Inventory Item */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description = FString();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Price = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Rarity = FString("Common");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEquipped = false;
};
