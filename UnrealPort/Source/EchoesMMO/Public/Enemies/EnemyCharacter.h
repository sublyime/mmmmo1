#pragma once

#include "CoreMinimal.h"
#include "Character/MMOCharacter.h"
#include "Core/GameDataTypes.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ECHOESMMO_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ===== Initialization =====
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void InitializeFromData(const FEnemyData& InEnemyData);

	// ===== Combat =====
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackTarget(class AMMOCharacter* Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnPlayerAggro(class AMMOCharacter* Aggressor);

	// ===== Rewards =====
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	int32 ExperienceReward = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	int32 GoldReward = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	FEnemyData EnemyData;

protected:
	UPROPERTY()
	class AMMOCharacter* CurrentTarget = nullptr;

	UPROPERTY()
	float TimeSinceLastAttack = 0.0f;

	UPROPERTY()
	float AttackCooldown = 2.0f;

	void UpdateAIBehavior(float DeltaTime);
	void PatrolBehavior();
	void ChaseBehavior(float DeltaTime);
	void AttackBehavior();
};
