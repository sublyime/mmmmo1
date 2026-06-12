# 📂 Echoes of the Fractured Realm - UE5 File Directory

## 📍 Quick Navigation

**First time?** Follow these steps:
1. Read [DELIVERY_SUMMARY.md](DELIVERY_SUMMARY.md) (5 min) - Overview of systems
2. Follow [QUICK_START.md](QUICK_START.md) (10 min) - Build & test  
3. Reference this file (FILE_DIRECTORY.md) when exploring code
4. Check [GAMEPLAY_FLOW.md](GAMEPLAY_FLOW.md) for game design
5. See [NETWORKING.md](NETWORKING.md) for multiplayer details

---

## 📚 Documentation Files Quick Reference

| File | Purpose | Time | For |
|------|---------|------|-----|
| **DELIVERY_SUMMARY.md** | Feature checklist & status | 5 min | Everyone |
| **README.md** | Architecture & C++ overview | 5 min | Developers |
| **QUICK_START.md** | Setup, build, first run | 10 min | First-timers |
| **FILE_DIRECTORY.md** | Code structure (this file) | 10 min | Developers |
| **GAMEPLAY_FLOW.md** | Game design & mechanics | 10 min | Designers |
| **NETWORKING.md** | Multiplayer & replication | 10 min | Network devs |

---

## 💾 Source Code Organization

### Core Game Logic (C++)

```
Source/EchoesMMO/Public/Core/
└── GameDataTypes.h
    • Enums: ECharacterClass, EFaction, EZone, EPvPStatus
    • Structs: FCharacterStats, FSkillData, FEnemyData, FPlayerProgression
    • Inventory: FInventoryItem
```

### Character System (C++)

```
Source/EchoesMMO/Public/Character/
└── MMOCharacter.h / .cpp
    • Player character class
    • Stats, HP/MP, progression
    • Combat actions (skills, basic attack)
    • Inventory management
    • Network replication

Public/Player/
└── MMOPlayerController.h / .cpp
    • Input handling
    • Character creation
    • Server RPC
    • UI management
```

### Combat System (C++)

```
Source/EchoesMMO/Public/Combat/
└── CombatSystem.h / .cpp
    • Turn-based encounter manager
    • Skill execution
    • Damage calculation
    • Combat state (rounds, turns)
    • Victory/defeat handling
    • Events for UI
```

### World & Game Management (C++)

```
Source/EchoesMMO/Public/Game/
└── MMOGameMode.h / .cpp
    • Zone database
    • Enemy database
    • Zone loading/unloading
    • Enemy spawning
    • Difficulty scaling
```

### Enemy & NPC Systems (C++)

```
Source/EchoesMMO/Public/Enemies/
├── EnemySpawner.h / .cpp
│   • Auto-spawning with cooldown
│   • Max active enemy limits
│   • Random spawn locations
│   • Dead enemy cleanup
│
└── EnemyCharacter.h / .cpp
    • Enemy AI (patrol, chase, attack)
    • Loot rewards
    • Level-based scaling
```

### Items & Loot (C++)

```
Source/EchoesMMO/Public/Items/
└── LootManager.h / .cpp
    • Loot generation by rarity
    • Drop rate management
    • Pickup system
    • Item collection
```

### UI System (UMG/C++)

```
Source/EchoesMMO/Public/UI/
├── MMOHUD.h / .cpp
│   • Main HUD actor
│   • Widget management
│
├── MainHUD.h / .cpp
│   • All screen transitions
│   • Title screen
│   • Character creation
│   • Game HUD
│   • Combat overlay
│   • Notifications
│   • Level-up banner
│
├── InventoryWidget.h / .cpp
│   • Item list display
│   • Item selection
│   • Use/drop functionality
│
└── WorldMapWidget.h / .cpp
    • Zone visualization
    • Zone info display
    • Travel button
```

### Module Setup

```
Source/EchoesMMO/
├── EchoesMMO.Build.cs
│   • Module build rules
│   • Dependencies: Core, Engine, UMG, Slate, OnlineSubsystem
│
└── Private/EchoesMMO.cpp
    • Module implementation
    • Log category definitions

Source/
├── EchoesMMO.Target.cs
│   • Game target configuration
│
└── EchoesMMOEditor.Target.cs
    • Editor target configuration
```

---

## 🎯 Important Files to Know

### When You Need To...

**Make gameplay changes:**
→ Edit `Source/EchoesMMO/Public/Character/MMOCharacter.h`

**Modify combat:**
→ Edit `Source/EchoesMMO/Public/Combat/CombatSystem.h`

**Add zones/enemies:**
→ Edit `Source/EchoesMMO/Private/Game/MMOGameMode.cpp`

**Change UI:**
→ Edit `Source/EchoesMMO/Public/UI/MainHUD.h`

**Add networking features:**
→ Reference `NETWORKING.md` then modify player controller

**Create Blueprint content:**
→ See `QUICK_START.md` section "Creating Blueprint Content"

---

## 📦 Build Outputs

After compilation, you'll have:

```
UnrealPort/
├── Binaries/          # Compiled .exe, .dll files
├── Intermediate/      # Build artifacts (auto-generated)
├── Saved/             # Game saves, logs
├── Plugins/           # Engine plugins (if added)
├── Content/           # Game assets (currently empty - add yours here)
└── EchoesMMO.sln      # Visual Studio solution
```

---

## 🚀 Workflow for Content Creation

### 1. Character Art
```
Create meshes in your 3D tool → Export FBX → Import to Content/Characters/
Create material → Assign to mesh
Create animations → Import
Create blueprint: BP_Warrior, BP_Mage, etc.
Assign mesh + animations + skills
```

### 2. Levels & Zones
```
Create new level: Content/Levels/MAP_Ironhaven
Drag base terrain mesh
Place enemy spawners
Place NPCs
Build lighting → Save
```

### 3. Items & Loot
```
Create data table → Row Structure: FSkillData
Add skill entries (Cleave, Fireball, etc.)
Link to character class
Loot system automatically uses items from tables
```

### 4. UI Polish
```
Customize MainHUD widget colors/fonts in UMG designer
Add splash images
Add sound cues for button clicks
Add animation transitions
```

---

## 🔗 Cross-References

These files reference each other:

- **MMOCharacter.h** uses `GameDataTypes.h` (structs)
- **CombatSystem.h** uses `MMOCharacter.h` (combat participants)
- **MMOGameMode.h** uses `GameDataTypes.h` (zone/enemy data)
- **EnemySpawner.h** uses `EnemyCharacter.h` & `MMOGameMode.h`
- **MainHUD.h** uses all character/combat/player classes
- **MMOPlayerController.h** uses `AMMOCharacter` & `AMMOHUD`

All include `EchoesMMO.h` for module definitions.

---

## 📋 Checklist for Getting Started

- [ ] Read DELIVERY_SUMMARY.md
- [ ] Read QUICK_START.md
- [ ] Generate Visual Studio project files
- [ ] Compile in Visual Studio
- [ ] Open EchoesMMO.uproject in UE5
- [ ] Wait for compilation (2-5 min)
- [ ] Create first Blueprint: BP_Warrior from AMMOCharacter
- [ ] Test in PIE (Play in Editor)
- [ ] Create character creation widget
- [ ] Add 3D mesh to character
- [ ] Customize game HUD
- [ ] Build world level
- [ ] Test multiplayer

---

## 📞 Finding Answers

**"How do I..."**

- ...get started? → QUICK_START.md
- ...understand the gameplay? → GAMEPLAY_FLOW.md
- ...set up multiplayer? → NETWORKING.md
- ...modify a class? → See class header in Source/
- ...understand the architecture? → README.md
- ...find a specific feature? → Use Ctrl+F in README.md

---

## 🎮 Test Checklist

After building, verify:

- [ ] Character creation works (test all 4 classes, 3 factions)
- [ ] Character loads in game world
- [ ] Combat initiates when encountering enemy
- [ ] Skills deal damage and apply effects
- [ ] Leveling up increases stats
- [ ] Zone travel works (test 2+ zones)
- [ ] Inventory accepts loot drops
- [ ] Multiplayer: 2 clients connect and see each other
- [ ] HUD displays current stats correctly
- [ ] Notifications appear on events

---

## 💡 Pro Tips

1. **Always compile after editing C++** - Use `Ctrl+Shift+B` in Visual Studio
2. **Blueprint changes save automatically** - No recompile needed
3. **Test in PIE first** - Faster iteration than full package
4. **Use Data Tables for content** - Easy to create without coding
5. **Check logs for errors** - Window → Developer Tools → Output Log
6. **Keep the editor open while programming** - Hot reload on Save (when possible)

---

**Happy Development! 🚀**

Next step: `QUICK_START.md` → Build → Create content
