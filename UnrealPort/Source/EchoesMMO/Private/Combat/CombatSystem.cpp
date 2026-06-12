#include "Combat/CombatSystem.h"
#include "Character/MMOCharacter.h"

ACombatSystem::ACombatSystem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ACombatSystem::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCombatActive || !PlayerCharacter || !EnemyCharacter)
	{
		return;
	}

	// Check if combat should end
	if (PlayerCharacter->GetCurrentHP() <= 0 || EnemyCharacter->GetCurrentHP() <= 0)
	{
		AMMOCharacter* Winner = PlayerCharacter->GetCurrentHP() > 0 ? PlayerCharacter : EnemyCharacter;
		AMMOCharacter* Loser = PlayerCharacter->GetCurrentHP() <= 0 ? PlayerCharacter : EnemyCharacter;
		EndCombat(Winner, Loser);
		return;
	}

	// Auto-cycle turns (simplified AI)
	if (!bPlayerTurn)
	{
		TurnTimerElapsed += DeltaTime;
		if (TurnTimerElapsed >= TurnDuration)
		{
			ProcessEnemyTurn();
			bPlayerTurn = true;
			TurnTimerElapsed = 0.0f;
		}
	}
}

void ACombatSystem::StartCombat(AMMOCharacter* Player, AMMOCharacter* Enemy)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	PlayerCharacter = Player;
	EnemyCharacter = Enemy;
	bCombatActive = true;
	bPlayerTurn = PlayerCharacter->BaseStats.GetSpeed() >= EnemyCharacter->BaseStats.GetSpeed();
	CurrentRound = 1;
	PlayerCombo = 0;
	TurnTimerElapsed = 0.0f;
}

void ACombatSystem::EndCombat(AMMOCharacter* Winner, AMMOCharacter* Loser)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	bCombatActive = false;

	if (Winner && Loser)
	{
		// Award winner
		Winner->Progression.TotalKills++;
		Winner->GainExperience(100 * Loser->Progression.Level);
		Winner->Progression.Gold += 50;

		// Penalize loser
		Loser->Progression.TotalDeaths++;
	}

	OnCombatEnded.Broadcast(Winner, Loser);

	// Destroy combat system after a delay
	GetWorld()->GetTimerManager().SetTimer(
		FTimerHandle(),
		[this]() { Destroy(); },
		2.0f,
		false
	);
}

void ACombatSystem::ExecuteAction(AMMOCharacter* Attacker, const FSkillData& Skill)
{
	if (GetLocalRole() != ROLE_Authority || !bCombatActive)
	{
		return;
	}

	if (!bPlayerTurn)
	{
		return; // Wait for player's turn
	}

	AMMOCharacter* Defender = (Attacker == PlayerCharacter) ? EnemyCharacter : PlayerCharacter;

	// Check mana cost
	if (Attacker->CurrentMP < Skill.ManaCost)
	{
		return;
	}

	// Consume mana
	Attacker->CurrentMP -= Skill.ManaCost;

	// Calculate and apply damage
	int32 Damage = CalculateDamage(Attacker, Defender, Skill);
	Defender->TakeDamage(Damage, Skill.DamageType);

	// Apply skill effects (buffs, debuffs, DoT, etc.)
	ApplySkillEffects(Defender, Skill);

	// Log action
	FString ActionLog = FString::Printf(TEXT("%s used %s for %d damage!"), *Attacker->CharacterName, *Skill.SkillName, Damage);
	OnCombatRound.Broadcast(Attacker, Defender, Damage, ActionLog);

	// Combo tracking
	PlayerCombo++;

	// Advance turn
	bPlayerTurn = false;
	TurnTimerElapsed = 0.0f;

	// Enemy will take turn automatically
}

void ACombatSystem::ExecuteBasicAttack(AMMOCharacter* Attacker, AMMOCharacter* Defender)
{
	if (GetLocalRole() != ROLE_Authority || !bCombatActive)
	{
		return;
	}

	// Basic attack uses weapon damage
	int32 BaseDamage = Attacker->BaseStats.GetAttack();
	int32 Damage = FMath::FloorToInt(BaseDamage * (0.8f + FMath::FRand() * 0.4f));
	Damage = Attacker->CalculateDamage(Damage, Defender->BaseStats.GetDefense(), EDamageType::Physical);

	Defender->TakeDamage(Damage, EDamageType::Physical);

	FString ActionLog = FString::Printf(TEXT("%s attacked %s for %d damage!"), *Attacker->CharacterName, *Defender->CharacterName, Damage);
	OnCombatRound.Broadcast(Attacker, Defender, Damage, ActionLog);

	bPlayerTurn = false;
	TurnTimerElapsed = 0.0f;
}

void ACombatSystem::ProcessEnemyTurn()
{
	if (!bCombatActive || !EnemyCharacter || !PlayerCharacter)
	{
		return;
	}

	// Simple AI: basic attack or random skill
	if (FMath::FRand() < 0.7f)
	{
		ExecuteBasicAttack(EnemyCharacter, PlayerCharacter);
	}
	else
	{
		// TODO: Use random skill from enemy skill list
		ExecuteBasicAttack(EnemyCharacter, PlayerCharacter);
	}
}

int32 ACombatSystem::CalculateDamage(AMMOCharacter* Attacker, AMMOCharacter* Defender, const FSkillData& Skill)
{
	int32 BaseDamage = FMath::RandRange(Skill.MinDamage, Skill.MaxDamage);
	return Attacker->CalculateDamage(BaseDamage, Defender->BaseStats.GetDefense(), Skill.DamageType);
}

void ACombatSystem::ApplySkillEffects(AMMOCharacter* Target, const FSkillData& Skill)
{
	// This would be extended to apply various skill effects like buffs, debuffs, DoT, etc.
	// For now, it's a placeholder
}

void ACombatSystem::AdvanceRound()
{
	CurrentRound++;
	PlayerCombo = 0;
}
