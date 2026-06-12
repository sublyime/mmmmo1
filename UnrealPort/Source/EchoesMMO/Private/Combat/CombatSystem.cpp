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

	if (!Player || !Enemy)
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
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		[this]() { Destroy(); },
		2.0f,
		false
	);
}

void ACombatSystem::ExecuteAction(AMMOCharacter* Attacker, const FSkillData& Skill)
{
	if (GetLocalRole() != ROLE_Authority || !bCombatActive || !Attacker)
	{
		return;
	}

	bool bIsPlayer = (Attacker == PlayerCharacter);
	if (bIsPlayer != bPlayerTurn)
	{
		return; // Not this character's turn
	}

	AMMOCharacter* Defender = bIsPlayer ? EnemyCharacter : PlayerCharacter;
	if (!Defender) return;

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

	if (bIsPlayer)
	{
		// Combo tracking only for player
		PlayerCombo++;
		
		// Advance turn
		bPlayerTurn = false;
		TurnTimerElapsed = 0.0f;
	}

	// Enemy will take turn automatically
}

void ACombatSystem::ExecuteBasicAttack(AMMOCharacter* Attacker, AMMOCharacter* Defender)
{
	if (GetLocalRole() != ROLE_Authority || !bCombatActive || !Attacker || !Defender)
	{
		return;
	}

	bool bIsPlayer = (Attacker == PlayerCharacter);
	if (bIsPlayer != bPlayerTurn)
	{
		return; // Not this character's turn
	}

	// Basic attack uses weapon damage
	int32 BaseDamage = Attacker->BaseStats.GetAttack();
	int32 Damage = FMath::FloorToInt(BaseDamage * (0.8f + FMath::FRand() * 0.4f));
	Damage = Attacker->CalculateDamage(Damage, Defender->BaseStats.GetDefense(), EDamageType::Physical);

	Defender->TakeDamage(Damage, EDamageType::Physical);

	FString ActionLog = FString::Printf(TEXT("%s attacked %s for %d damage!"), *Attacker->CharacterName, *Defender->CharacterName, Damage);
	OnCombatRound.Broadcast(Attacker, Defender, Damage, ActionLog);

	if (bIsPlayer)
	{
		PlayerCombo++;
		bPlayerTurn = false;
		TurnTimerElapsed = 0.0f;
	}
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
		bool bSkillExecuted = false;

		if (EnemyCharacter->AvailableSkills.Num() > 0)
		{
			// Filter for skills the enemy actually has enough mana to cast
			TArray<int32> ValidSkillIndices;
			for (int32 i = 0; i < EnemyCharacter->AvailableSkills.Num(); ++i)
			{
				if (EnemyCharacter->CurrentMP >= EnemyCharacter->AvailableSkills[i].ManaCost)
				{
					ValidSkillIndices.Add(i);
				}
			}

			if (ValidSkillIndices.Num() > 0)
			{
				int32 RandomIndex = ValidSkillIndices[FMath::RandRange(0, ValidSkillIndices.Num() - 1)];
				ExecuteAction(EnemyCharacter, EnemyCharacter->AvailableSkills[RandomIndex]);
				bSkillExecuted = true;
			}
		}
		
		if (!bSkillExecuted)
		{
			ExecuteBasicAttack(EnemyCharacter, PlayerCharacter); // Fallback
		}
	}
}

int32 ACombatSystem::CalculateDamage(AMMOCharacter* Attacker, AMMOCharacter* Defender, const FSkillData& Skill)
{
	int32 BaseDamage = FMath::RandRange(Skill.MinDamage, Skill.MaxDamage);
	return Attacker->CalculateDamage(BaseDamage, Defender->BaseStats.GetDefense(), Skill.DamageType);
}

void ACombatSystem::ApplySkillEffects(AMMOCharacter* Target, const FSkillData& Skill)
{
	// Placeholder implementation concept:
	// if (Target && Skill.EffectDuration > 0) {
	//     FCombatEffect NewEffect;
	//     NewEffect.Type = Skill.EffectType;
	//     NewEffect.Duration = Skill.EffectDuration;
	//     Target->ActiveEffects.Add(NewEffect);
	// }
}

void ACombatSystem::AdvanceRound()
{
	CurrentRound++;
	PlayerCombo = 0;
}
