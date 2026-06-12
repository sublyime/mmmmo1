#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/GameDataTypes.h"
#include "MMOCharacter.generated.h"

UCLASS()
class ECHOESMMO_API AMMOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Core Identity ---
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FString CharacterName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Identity")
	ECharacterClass CurrentClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Identity")
	EFaction CurrentFaction;

	// --- Stats & Progression ---
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 Level;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 CurrentXP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 Gold;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 CurrentHP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 CurrentMP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FCharacterStats BaseStats;

	// --- Server Combat Functions ---
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Combat")
	void Server_TakeDamage(int32 DamageAmount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Combat")
	void Server_AddXP(int32 XPAmount);

	UFUNCTION(Client, Reliable)
	void Client_OnLevelUp(int32 NewLevel);

	void CheckLevelUp();
};