#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Core/GameDataTypes.h"
#include "MMOCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AMMOPlayerController;

UCLASS()
class ECHOESMMO_API AMMOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMOCharacter();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	// ===== Character Data =====
	UPROPERTY(BlueprintReadWrite, Replicated)
	FString CharacterName = FString(TEXT("Hero"));

	UPROPERTY(BlueprintReadWrite, Replicated)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintReadWrite, Replicated)
	EFaction Faction = EFaction::IronveilDominion;

	UPROPERTY(BlueprintReadWrite, Replicated)
	EZone CurrentZone = EZone::IronhavenCity;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FCharacterStats BaseStats;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FPlayerProgression Progression;

	// ===== Combat Data =====
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 CurrentHP = 1000;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 MaxHP = 1000;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 CurrentMP = 500;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 MaxMP = 500;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FCombatEffect> ActiveEffects;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FInventoryItem> Inventory;

	// ===== Combat System =====
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(int32 DamageAmount, EDamageType DamageType = EDamageType::Physical);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Heal(int32 HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RestoreMana(int32 ManaAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	int32 CalculateDamage(int32 BaseDamage, int32 TargetDefense, EDamageType DamageType) const;

	// ===== Progression =====
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void GainExperience(int32 ExperienceAmount);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void LevelUp();

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetCurrentHP() const { return CurrentHP; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	int32 GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	float GetHealthPercent() const { return MaxHP > 0 ? (float)CurrentHP / MaxHP : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Progression")
	float GetManaPercent() const { return MaxMP > 0 ? (float)CurrentMP / MaxMP : 0.0f; }

	// ===== Zone Travel =====
	UFUNCTION(BlueprintCallable, Category = "World")
	void TravelToZone(EZone TargetZone);

	UFUNCTION(BlueprintCallable, Category = "World")
	bool CanTravelToZone(EZone TargetZone) const;

	// ===== Inventory =====
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(const FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(const FString& ItemName, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(const FString& ItemName, int32 RequiredQuantity = 1) const;

	// ===== Effects =====
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddEffect(const FCombatEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveEffect(const FString& EffectName);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TickEffects();

	// ===== Network =====
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ===== Input Callbacks =====
	void OnLook(const FInputActionValue& Value);
	void OnMove(const FInputActionValue& Value);
	void OnInteract();
	void OnAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	FVector2D LookAxisVector = FVector2D::ZeroVector;
	FVector2D MoveAxisVector = FVector2D::ZeroVector;
};
