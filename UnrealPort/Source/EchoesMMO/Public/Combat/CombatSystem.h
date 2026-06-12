#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/GameDataTypes.h"
#include "CombatSystem.generated.h"

class AMMOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_Four(FOnCombatRound, AMMOCharacter*, Attacker, AMMOCharacter*, Defender, int32, Damage, FString, ActionLog);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_Two(FCombatEndedDelegate, AMMOCharacter*, Winner, AMMOCharacter*, Loser);

UCLASS()
class ECHOESMMO_API ACombatSystem : public AActor
{
	GENERATED_BODY()

public:
	ACombatSystem();

	virtual void Tick(float DeltaTime) override;

	// ===== Combat Management =====
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartCombat(AMMOCharacter* PlayerCharacter, AMMOCharacter* EnemyCharacter);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndCombat(AMMOCharacter* Winner, AMMOCharacter* Loser);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteAction(AMMOCharacter* Attacker, const FSkillData& Skill);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteBasicAttack(AMMOCharacter* Attacker, AMMOCharacter* Defender);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsCombatActive() const { return bCombatActive; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	int32 GetCurrentRound() const { return CurrentRound; }

	// ===== Events =====
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCombatRound OnCombatRound;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCombatEndedDelegate OnCombatEnded;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AMMOCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	AMMOCharacter* EnemyCharacter = nullptr;

	UPROPERTY()
	bool bCombatActive = false;

	UPROPERTY()
	bool bPlayerTurn = true;

	UPROPERTY()
	int32 CurrentRound = 1;

	UPROPERTY()
	int32 PlayerCombo = 0;

	UPROPERTY()
	float TurnTimerElapsed = 0.0f;

	UPROPERTY()
	const float TurnDuration = 3.0f;

	void ProcessEnemyTurn();
	void AdvanceRound();
	int32 CalculateDamage(AMMOCharacter* Attacker, AMMOCharacter* Defender, const FSkillData& Skill);
	void ApplySkillEffects(AMMOCharacter* Target, const FSkillData& Skill);
};
