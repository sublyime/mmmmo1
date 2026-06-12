#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameDataTypes.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior     UMETA(DisplayName = "Warbringer"),
	Mage        UMETA(DisplayName = "Arcanist"),
	Rogue       UMETA(DisplayName = "Shadowblade"),
	Hunter      UMETA(DisplayName = "Voidhunter")
};

UENUM(BlueprintType)
enum class EFaction : uint8
{
	None        UMETA(DisplayName = "None"),
	Ironveil    UMETA(DisplayName = "Ironveil Dominion"),
	Sunken      UMETA(DisplayName = "The Sunken Court"),
	Voidborn    UMETA(DisplayName = "Voidborn Conclave")
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxMP = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Attack = 75;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Defense = 35;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Speed = 10;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 CooldownTurns = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 BaseDamage = 0;
};