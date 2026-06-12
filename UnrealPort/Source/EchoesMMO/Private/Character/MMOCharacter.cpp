#include "Character/MMOCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AMMOCharacter::AMMOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	// Default Start Values
	Level = 1;
	CurrentXP = 0;
	Gold = 1250;
	CurrentHP = BaseStats.MaxHP;
	CurrentMP = BaseStats.MaxMP;
}

void AMMOCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		CurrentHP = BaseStats.MaxHP;
		CurrentMP = BaseStats.MaxMP;
	}
}

void AMMOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMMOCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMMOCharacter, CharacterName);
	DOREPLIFETIME(AMMOCharacter, CurrentClass);
	DOREPLIFETIME(AMMOCharacter, CurrentFaction);
	DOREPLIFETIME(AMMOCharacter, Level);
	DOREPLIFETIME(AMMOCharacter, CurrentXP);
	DOREPLIFETIME(AMMOCharacter, Gold);
	DOREPLIFETIME(AMMOCharacter, CurrentHP);
	DOREPLIFETIME(AMMOCharacter, CurrentMP);
	DOREPLIFETIME(AMMOCharacter, BaseStats);
}

bool AMMOCharacter::Server_TakeDamage_Validate(int32 DamageAmount) { return true; }
void AMMOCharacter::Server_TakeDamage_Implementation(int32 DamageAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, BaseStats.MaxHP);
	if (CurrentHP <= 0)
	{
		// Handle death state
	}
}

bool AMMOCharacter::Server_AddXP_Validate(int32 XPAmount) { return true; }
void AMMOCharacter::Server_AddXP_Implementation(int32 XPAmount)
{
	CurrentXP += XPAmount;
	CheckLevelUp();
}

void AMMOCharacter::CheckLevelUp()
{
	// Base XP scaling logic
	int32 XPNeeded = 100 * FMath::Pow(1.5f, Level - 1);
	// Level up sequence loop goes here
}

void AMMOCharacter::Client_OnLevelUp_Implementation(int32 NewLevel)
{
	// Play Level Up FX and update UI locally
}