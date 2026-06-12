# Networking & Multiplayer Setup

## Overview

Echoes MMO uses **Unreal Engine's built-in replication system** for networked multiplayer. All critical game state is replicated from server to clients.

## Network Architecture

### Server Authority Model
```
Client A ──┐
           │─→ [SERVER] ←─ Client B
Client C ──┘
```

- **Server**: Has authoritative game state
- **Clients**: Send input/requests to server via RPC
- **Replication**: Server broadcasts state changes to all clients

## Replicated Data

### AMMOCharacter
```cpp
UPROPERTY(Replicated)
FString CharacterName;

UPROPERTY(Replicated)
int32 CurrentHP;

UPROPERTY(Replicated)
int32 CurrentMP;

UPROPERTY(Replicated)
EZone CurrentZone;

UPROPERTY(Replicated)
FPlayerProgression Progression;

UPROPERTY(Replicated)
TArray<FCombatEffect> ActiveEffects;

UPROPERTY(Replicated)
TArray<FInventoryItem> Inventory;
```

### Replication Update Frequency
- **Frequency**: 10 updates per second (default)
- **Adjust in Project Settings** → Engine → Networking

## Server RPC Examples

### Character Creation
```cpp
UFUNCTION(Server, Reliable, WithValidation)
void Server_CreateCharacter(
    const FString& CharacterName, 
    ECharacterClass SelectedClass, 
    EFaction SelectedFaction);
```

**Flow**:
1. Client calls `CreateCharacter()`
2. RPC sent to server
3. Server validates data
4. Server spawns character
5. Server calls replication update
6. All clients receive character state

### Combat Actions
```cpp
UFUNCTION(Server, Reliable)
void Server_UseSkill(int32 SkillIndex);

UFUNCTION(NetMulticast, Unreliable)
void Multicast_PlayAttackAnimation();
```

**Types**:
- `Server`: Only server executes
- `Client`: Only owning client executes
- `Multicast`: All clients execute

**Reliability**:
- `Reliable`: Guaranteed delivery (slower)
- `Unreliable`: Fire-and-forget (faster, for animations/effects)

## Setting Up Multiplayer Test

### Local Network Play

**Project Settings**:
```
Project Settings → Engine → Multiplayer
├─ Number of Players: 2
├─ Network Mode: Listen Server
└─ Max Spectators: 2
```

**Launch Configuration**:

1. **Create Standalone Package**
   ```
   File → Package Project → Windows (64-bit)
   ```

2. **Run First Instance (Server)**
   ```bash
   EchoesMMO.exe
   ```

3. **Run Second Instance (Client)**
   ```bash
   EchoesMMO.exe -join 127.0.0.1
   ```

### Dedicated Server (Advanced)

Create `EchoesMMOServer.Target.cs`:
```cpp
public class EchoesMMOServerTarget : TargetRules
{
    public EchoesMMOServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        // ... other settings
    }
}
```

Compile as server build:
```bash
Build → Build Solution
(Choose EchoesMMO-Server config)
```

## Replication Graph (Optimization)

For large player counts, use Replication Graph to control what data each player sees:

**Enable Replication Graph**:
```
Project Settings → Engine → Replication Graph
├─ Use Replication Graph: True
├─ Replication Graph Class: [Custom or default]
└─ Max Spawn Channel Reservations: 256
```

**Culling Zones** (only replicate nearby players):
```cpp
void AMMOCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Only replicate to players in same zone
    DOREPLIFETIME_CONDITION(AMMOCharacter, CurrentHP, COND_Custom);
    DOREPLIFETIME_CONDITION(AMMOCharacter, CurrentZone, COND_OwnerOnly);
}
```

## Common Networking Patterns

### 1. **Character Movement**
```cpp
// Auto-replicated via SetReplicateMovement(true)
GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
```

### 2. **Combat Damage**
```cpp
// Server-only (authority check)
void AMMOCharacter::TakeDamage(int32 Damage)
{
    if (GetLocalRole() != ROLE_Authority) return;
    
    CurrentHP -= Damage; // Replicates to all clients
}
```

### 3. **Client Prediction**
```cpp
void AMMOCharacter::OnMove(const FInputActionValue& Value)
{
    // Client-side: immediate visual feedback
    AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
    
    // Tell server
    if (IsLocallyControlled())
    {
        Server_UpdateMovement(Value);
    }
}
```

### 4. **Reliable State Changes**
```cpp
// Use Reliable for important data
UFUNCTION(Server, Reliable, WithValidation)
void Server_LevelUp();

bool AMMOCharacter::Server_LevelUp_Validate()
{
    // Prevent cheating
    return Progression.CurrentExperience >= Progression.ExperienceToNextLevel;
}
```

## Bandwidth Optimization

### Property Replication Conditions
```cpp
UPROPERTY(Replicated)
int32 InternalCounter; // Every frame

UPROPERTY(ReplicatedUsing=OnRep_Health)
int32 Health; // Only when changed

UPROPERTY()
int32 CachedValue; // Not replicated
```

### Importance Levels
```
ROLE_Authority     → Server
ROLE_AutonomousProxy → Owner's client
ROLE_SimulatedProxy → Other clients
```

### Data Compression
Use bitfields for small values:
```cpp
UPROPERTY(Replicated)
uint8 CurrentLevel : 8;  // 0-255 range

UPROPERTY(Replicated)
uint16 CurrentHP : 16;   // 0-65535 range
```

## Debugging Networking

### Enable Network Logging
```
Project Settings → Engine → General Settings
├─ Logging
└─ Add "LogNet" with Warning verbosity
```

### Stat Commands (in-game console)
```
stat net          // Network statistics
stat unitgraph    // Unit properties
stat hitches      // Frame rate issues
```

### Network Debugging UI
```cpp
// In game.cpp or debug widget
if (GEngine)
{
    GEngine->AddOnScreenDebugMessage(
        -1, 
        5.0f, 
        FColor::Green, 
        FString::Printf(
            TEXT("Role: %d | Ping: %d ms"), 
            (int32)GetLocalRole(), 
            GetPlayerController()->PlayerState->GetPing()
        )
    );
}
```

## Online Subsystems (Optional - for Steam/EOS)

### Enable Steam Support
1. **Plugins** → Search "Online Subsystem"
   - Enable: `Online Subsystem Steam`
   - Enable: `Online Subsystem NULL` (fallback)

2. **Edit DefaultEngine.ini**
   ```ini
   [OnlineSubsystem]
   DefaultPlatformService=Null
   
   [OnlineSubsystemSteam]
   bEnabled=true
   SteamDevAppId=480
   ```

3. **Connect Players**
   ```cpp
   IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
   IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
   ```

## Performance Targets

| Metric | Target | Max |
|--------|--------|-----|
| Replication Rate | 10 Hz | 60 Hz |
| Bandwidth per Player | 10 KB/s | 50 KB/s |
| Latency | <100 ms | <250 ms |
| Player Count (per server) | 100+ | 500+ |

## Troubleshooting

### Characters Not Replicating
→ Check `bReplicates = true` on Actor
→ Ensure `GetLifetimeReplicatedProps()` is implemented
→ Verify `DOREPLIFETIME` macros

### Desync Issues
→ Use Server authority for all gameplay logic
→ Avoid client-side state changes
→ Use server validation in RPC functions

### High Bandwidth
→ Reduce replication rate
→ Implement client-side prediction
→ Use replication conditions (COND_OwnerOnly, etc.)

---

**Multiplayer is now ready to scale from 2 to 100+ players!**
