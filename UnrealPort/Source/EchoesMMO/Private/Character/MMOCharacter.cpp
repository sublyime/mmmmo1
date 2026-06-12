#include "Character/MMOCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"

AMMOCharacter::AMMOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// Character configuration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 500.0f;

	// Don't rotate character with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Initialize stats based on class
	BaseStats.Strength = 10;
	BaseStats.Intelligence = 10;
	BaseStats.Vitality = 10;
	BaseStats.Agility = 10;
	BaseStats.Dexterity = 10;
	BaseStats.Wisdom = 10;
}

void AMMOCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize HP and MP based on stats
	MaxHP = BaseStats.GetHealthPoints();
	CurrentHP = MaxHP;
	MaxMP = BaseStats.GetManaPoints();
	CurrentMP = MaxMP;

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMMOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMOCharacter::OnMove);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMOCharacter::OnLook);
		}

		// Interact
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMMOCharacter::OnInteract);
		}

		// Attack
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMMOCharacter::OnAttack);
		}
	}
}

void AMMOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		TickEffects();
	}
}

void AMMOCharacter::OnMove(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
		AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);
	}
}

void AMMOCharacter::OnLook(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.0f)
	{
		AddControllerYawInput(Value.Get<FVector2D>().X);
		AddControllerPitchInput(Value.Get<FVector2D>().Y);
	}
}

void AMMOCharacter::OnInteract()
{
	// Interact with NPCs, objects, etc.
}

void AMMOCharacter::OnAttack()
{
	// Trigger attack animation and combat system
}

void AMMOCharacter::TakeDamage(int32 DamageAmount, EDamageType DamageType)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		int32 ActualDamage = DamageAmount;

		// Apply defense reduction
		int32 Defense = BaseStats.GetDefense();
		ActualDamage = FMath::Max(1, ActualDamage - (Defense / 2));

		CurrentHP = FMath::Max(0, CurrentHP - ActualDamage);

		if (CurrentHP <= 0)
		{
			Progression.TotalDeaths++;
			// Trigger death sequence
		}
	}
}

void AMMOCharacter::Heal(int32 HealAmount)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHP = FMath::Min(MaxHP, CurrentHP + HealAmount);
	}
}

void AMMOCharacter::RestoreMana(int32 ManaAmount)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentMP = FMath::Min(MaxMP, CurrentMP + ManaAmount);
	}
}

int32 AMMOCharacter::CalculateDamage(int32 BaseDamage, int32 TargetDefense, EDamageType DamageType) const
{
	float Multiplier = 1.0f;

	switch (DamageType)
	{
	case EDamageType::Physical:
		Multiplier = 1.0f;
		break;
	case EDamageType::Magic:
		Multiplier = 1.5f;
		break;
	case EDamageType::Poison:
		Multiplier = 0.8f;
		break;
	}

	int32 Damage = FMath::Max(1, FMath::FloorToInt(BaseDamage * Multiplier * (1.0f - TargetDefense * 0.0008f)));
	return Damage;
}

void AMMOCharacter::GainExperience(int32 ExperienceAmount)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Progression.CurrentExperience += ExperienceAmount;

		while (Progression.CurrentExperience >= Progression.ExperienceToNextLevel)
		{
			LevelUp();
		}
	}
}

void AMMOCharacter::LevelUp()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Progression.CurrentExperience -= Progression.ExperienceToNextLevel;
		Progression.Level++;
		Progression.ExperienceToNextLevel = FMath::FloorToInt(Progression.ExperienceToNextLevel * 1.5f);

		// Increase base stats
		const float StatBoost = 1.12f;
		MaxHP = FMath::FloorToInt(MaxHP * StatBoost);
		MaxMP = FMath::FloorToInt(MaxMP * StatBoost);
		BaseStats.Strength = FMath::FloorToInt(BaseStats.Strength * StatBoost);
		BaseStats.Intelligence = FMath::FloorToInt(BaseStats.Intelligence * StatBoost);

		CurrentHP = MaxHP;
		CurrentMP = MaxMP;
	}
}

void AMMOCharacter::TravelToZone(EZone TargetZone)
{
	if (GetLocalRole() == ROLE_Authority && CanTravelToZone(TargetZone))
	{
		CurrentZone = TargetZone;
		// TODO: Trigger zone loading and character teleportation
	}
}

bool AMMOCharacter::CanTravelToZone(EZone TargetZone) const
{
	// Check level requirements, etc.
	return true;
}

void AMMOCharacter::AddItemToInventory(const FInventoryItem& Item)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (FInventoryItem& ExistingItem : Inventory)
		{
			if (ExistingItem.ItemName == Item.ItemName)
			{
				ExistingItem.Quantity += Item.Quantity;
				return;
			}
		}
		Inventory.Add(Item);
	}
}

void AMMOCharacter::RemoveItemFromInventory(const FString& ItemName, int32 Quantity)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (int32 i = 0; i < Inventory.Num(); ++i)
		{
			if (Inventory[i].ItemName == ItemName)
			{
				Inventory[i].Quantity -= Quantity;
				if (Inventory[i].Quantity <= 0)
				{
					Inventory.RemoveAt(i);
				}
				break;
			}
		}
	}
}

bool AMMOCharacter::HasItem(const FString& ItemName, int32 RequiredQuantity) const
{
	for (const FInventoryItem& Item : Inventory)
	{
		if (Item.ItemName == ItemName && Item.Quantity >= RequiredQuantity)
		{
			return true;
		}
	}
	return false;
}

void AMMOCharacter::AddEffect(const FCombatEffect& Effect)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ActiveEffects.Add(Effect);
	}
}

void AMMOCharacter::RemoveEffect(const FString& EffectName)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (int32 i = 0; i < ActiveEffects.Num(); ++i)
		{
			if (ActiveEffects[i].EffectName == EffectName)
			{
				ActiveEffects.RemoveAt(i);
				break;
			}
		}
	}
}

void AMMOCharacter::TickEffects()
{
	for (FCombatEffect& Effect : ActiveEffects)
	{
		Effect.RemainingTurns--;
	}

	ActiveEffects.RemoveAll([](const FCombatEffect& Effect)
		{
			return Effect.RemainingTurns <= 0;
		});
}

void AMMOCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMMOCharacter, CharacterName);
	DOREPLIFETIME(AMMOCharacter, CharacterClass);
	DOREPLIFETIME(AMMOCharacter, Faction);
	DOREPLIFETIME(AMMOCharacter, CurrentZone);
	DOREPLIFETIME(AMMOCharacter, BaseStats);
	DOREPLIFETIME(AMMOCharacter, Progression);
	DOREPLIFETIME(AMMOCharacter, CurrentHP);
	DOREPLIFETIME(AMMOCharacter, MaxHP);
	DOREPLIFETIME(AMMOCharacter, CurrentMP);
	DOREPLIFETIME(AMMOCharacter, MaxMP);
	DOREPLIFETIME(AMMOCharacter, ActiveEffects);
	DOREPLIFETIME(AMMOCharacter, Inventory);
}
