#include "Enemies/EnemyCharacter.h"
#include "Character/MMOCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// Basic configuration
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health
	CurrentHP = EnemyData.Stats.GetHealthPoints();
	MaxHP = CurrentHP;
	CurrentMP = EnemyData.Stats.GetManaPoints();
	MaxMP = CurrentMP;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		UpdateAIBehavior(DeltaTime);
		TimeSinceLastAttack += DeltaTime;
	}
}

void AEnemyCharacter::InitializeFromData(const FEnemyData& InEnemyData)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	EnemyData = InEnemyData;
	CharacterName = InEnemyData.EnemyName;
	ExperienceReward = InEnemyData.ExperienceReward;
	GoldReward = InEnemyData.GoldReward;
	BaseStats = InEnemyData.Stats;

	// Apply level scaling
	float LevelMultiplier = 1.0f + (InEnemyData.Level - 1) * 0.15f;
	MaxHP = FMath::FloorToInt(BaseStats.GetHealthPoints() * LevelMultiplier);
	CurrentHP = MaxHP;
	MaxMP = FMath::FloorToInt(BaseStats.GetManaPoints() * LevelMultiplier);
	CurrentMP = MaxMP;
}

void AEnemyCharacter::AttackTarget(AMMOCharacter* Target)
{
	if (!Target || TimeSinceLastAttack < AttackCooldown)
	{
		return;
	}

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Calculate damage
	int32 BaseDamage = EnemyData.Stats.GetAttack();
	int32 Damage = FMath::FloorToInt(BaseDamage * (0.8f + FMath::FRand() * 0.4f));
	Damage = CalculateDamage(Damage, Target->BaseStats.GetDefense(), EDamageType::Physical);

	Target->TakeDamage(Damage, EDamageType::Physical);
	TimeSinceLastAttack = 0.0f;
}

void AEnemyCharacter::OnPlayerAggro(AMMOCharacter* Aggressor)
{
	CurrentTarget = Aggressor;
}

void AEnemyCharacter::UpdateAIBehavior(float DeltaTime)
{
	if (CurrentTarget && CurrentTarget->GetCurrentHP() > 0)
	{
		ChaseBehavior(DeltaTime);
		AttackBehavior();
	}
	else
	{
		PatrolBehavior();
	}
}

void AEnemyCharacter::PatrolBehavior()
{
	// Simple patrol - can be enhanced with waypoints
	// For now, just idle
}

void AEnemyCharacter::ChaseBehavior(float DeltaTime)
{
	if (!CurrentTarget)
	{
		return;
	}

	FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddMovementInput(DirectionToTarget, 1.0f);
}

void AEnemyCharacter::AttackBehavior()
{
	if (!CurrentTarget || CurrentTarget->GetCurrentHP() <= 0)
	{
		return;
	}

	float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	if (DistanceToTarget < 200.0f)
	{
		AttackTarget(CurrentTarget);
	}
}
