# Quick Start Guide - Echoes of the Fractured Realm (UE5)

## Setup Instructions

### 1. **Download & Generate Project**
```bash
cd UnrealPort
right-click EchoesMMO.uproject → Generate Visual Studio project files
```

### 2. **Compile**
```bash
Open EchoesMMO.sln in Visual Studio 2022+
Build → Build Solution
```

### 3. **Open in UE5**
```bash
Double-click EchoesMMO.uproject
Wait for editor to load and compile
```

## Creating Blueprint Content

### Character Classes (In UE5 Editor)

1. **Create Character Blueprint**
   - Content Browser → New → Blueprint Class
   - Parent Class: `AMMOCharacter`
   - Name: `BP_Warrior`, `BP_Mage`, etc.

2. **Set Up Class Stats**
   - In Blueprint Details, set `Base Stats`:
     - **Warrior**: STR 18, VIT 16, AGI 10, INT 6
     - **Mage**: INT 20, WIS 18, STR 6, VIT 8
     - **Rogue**: AGI 20, DEX 18, STR 12, VIT 8
     - **Hunter**: DEX 20, AGI 16, INT 12, VIT 10

3. **Assign Skeletal Mesh**
   - Select mesh in Details panel
   - Add animations (walking, attacking, idle)

### Skills & Combat

1. **Create Skill Data Table**
   - Content Browser → New → Data Table
   - Row Structure: `FSkillData`
   - Add skills: Cleave, Fireball, Backstab, Void Shot, etc.

2. **Link to Character Class**
   - In `BP_Warrior`, assign skill data table to loadout

### Zones & Levels

1. **Create Level for Each Zone**
   - File → New Level → Blank Level
   - Name: `MAP_Ironhaven`, `MAP_AshenWastes`, etc.

2. **Place Zone Spawners**
   - Drag `BP_EnemySpawner` actor
   - Configure zone type and enemy count

3. **Add NPCs**
   - Drag merchant/quest-giver blueprints
   - Configure dialogue and interactions

## Running the Game

### Single Player (PIE - Play in Editor)
```
Press ALT + P or click Play button
- Test character creation
- Test combat mechanics
- Test zone travel
```

### Multiplayer (Local)
```
1. Edit → Project Settings → Engine → Multiplayer
   - Set "Number of Players" = 2
2. File → Package Project → Windows (64-bit)
3. Run packaged game
4. First instance: Host
5. Second instance: Connect to host
```

## Complete Feature Checklist

### ✅ Completed
- [x] Core character system (C++)
- [x] Combat system framework (C++)
- [x] Zone management (C++)
- [x] Inventory system (C++)
- [x] Progression & leveling (C++)
- [x] Network replication (C++)
- [x] Basic UI widgets (UMG)
- [x] Player controller setup (C++)

### ⏳ In Progress
- [ ] Create Blueprint character variants
- [ ] Add 3D meshes & animations
- [ ] Build world levels
- [ ] Implement skill system UI
- [ ] Add combat visual effects

### 📋 TODO
- [ ] Enemy AI (simple patrol, aggro, combat)
- [ ] NPC system (merchants, quest-givers)
- [ ] PvP arena matchmaking
- [ ] Guild system
- [ ] Marketplace/trading
- [ ] Leaderboards
- [ ] Audio/music
- [ ] Advanced VFX

## C++ to Blueprint Bridge

All game logic is in C++. Customize via Blueprints:

```
AMMOCharacter (C++)
    ↓
    BP_Warrior (Blueprint)
    │
    ├─ Skeletal Mesh
    ├─ Animations
    ├─ VFX
    └─ Particle Systems
```

### Key Classes to Blueprint

| C++ Class | Blueprint Path | Purpose |
|-----------|----------------|---------|
| `AMMOCharacter` | `BP_Warrior`, etc | Playable character |
| `ACombatSystem` | Auto-spawned | Combat encounters |
| `AMMOGameMode` | Set as default | World & game rules |
| `AMMOPlayerController` | Set as default | Input & UI |
| `AMMOHUD` | Set as default | HUD display |

## Testing Checklist

- [ ] Character creation works (name, class, faction)
- [ ] Character loads with correct stats
- [ ] Combat initiates (player vs enemy)
- [ ] Damage calculation works
- [ ] Level up triggers at correct XP
- [ ] Zone travel succeeds
- [ ] Inventory adds/removes items
- [ ] Multiplayer character replication works

## Common Issues & Fixes

### "Missing blueprint parent class error"
→ Recompile C++ project and refresh editor

### "HUD not showing"
→ Assign `AMMOHUD` in Project Settings → Maps & Modes → Default HUD

### "Character not moving"
→ Check Enhanced Input plugin is enabled

### "Compilation errors"
→ Clean intermediate files and rebuild:
   ```bash
   Delete Intermediate/ folder
   Right-click .uproject → Generate Visual Studio project files
   Rebuild in Visual Studio
   ```

## File Structure for Reference

```
UnrealPort/Source/EchoesMMO/
├── Public/
│   ├── Core/
│   │   └── GameDataTypes.h       ← All structs & enums
│   ├── Character/
│   │   └── MMOCharacter.h        ← Player class
│   ├── Combat/
│   │   └── CombatSystem.h        ← Combat engine
│   ├── Game/
│   │   └── MMOGameMode.h         ← World management
│   ├── Player/
│   │   └── MMOPlayerController.h ← Input & RPC
│   ├── UI/
│   │   ├── MainHUD.h             ← Main HUD widget
│   │   ├── MMOHUD.h              ← HUD actor
│   │   ├── InventoryWidget.h     ← Inventory UI
│   │   └── WorldMapWidget.h      ← World map UI
│   └── EchoesMMO.h               ← Module header
└── Private/
    └── [corresponding .cpp files]
```

## Next Development Steps

1. **Create Character Meshes**
   - Acquire/create 3D models for each class
   - Set up skeletal meshes in UE5

2. **Implement Animations**
   - Attack, cast, walk, idle, death
   - Transition states

3. **Build World Levels**
   - Create 7 explorable zones
   - Place enemies, NPCs, loot

4. **Combat Polish**
   - Particle effects for spells
   - Sound effects
   - Screen shake on hits

5. **Advanced Features**
   - Enemy AI (patrolling, aggression ranges)
   - Loot system
   - Quests
   - Player guilds

---

**Status**: Full gameplay foundation ready. Now building Blueprint content!
