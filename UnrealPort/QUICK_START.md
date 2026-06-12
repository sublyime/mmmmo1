# Quick Start Guide - Echoes of the Fractured Realm (UE5) 🚀

Get up and running in **10 minutes** on Windows. Linux users see "Linux Setup" section below.

## Step 1: Download & Generate Project (2 min)

### Windows
```bash
cd UnrealPort
right-click EchoesMMO.uproject
select "Generate Visual Studio project files"
```

**Expected**: A new `EchoesMMO.sln` file appears in the folder.

### Linux
```bash
cd UnrealPort
/path/to/UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh
```

## Step 2: Compile (3-5 min)

### Windows
```bash
Open EchoesMMO.sln in Visual Studio 2022
Select "Development Editor" configuration (dropdown, top-left)
Build → Build Solution (or Ctrl+Shift+B)
Wait for build to complete (watch Output window)
```

**Expected**: "Build succeeded" message. .exe files appear in `Binaries/Win64/`.

### Linux
```bash
make EchoesMMOEditor-Linux-Development
```

## Step 3: Open in UE5 Editor (2 min)

```bash
Double-click EchoesMMO.uproject
# OR from command line:
/path/to/UnrealEngine/Engine/Binaries/Win64/UE4Editor EchoesMMO.uproject
```

**Expected**: Unreal Editor opens, loads the project, and shows the default level.

## ⚙️ VS Code C++ IntelliSense Setup (Optional)

If you're using **VS Code** for editing C++ code:

1. **Verify `.vscode/c_cpp_properties.json` exists**
   - Should be auto-created when project generates
   - If missing, create it at workspace root

2. **Reload IntelliSense**
   - `Ctrl+Shift+P` → "Rebuild IntelliSense"
   - Or close & reopen VS Code

**Note**: IntelliSense for engine headers is limited. Use Visual Studio 2022 for full IntelliSense when doing heavy C++ work.

## Creating Blueprint Content

### Character Classes (In UE5 Editor)

1. **Create Character Blueprint**
   - Content Browser → New → Blueprint Class
   - Parent Class: `AMMOCharacter`
   - Name: `BP_Warrior`, `BP_Mage`, etc.

2. **Set Up Class Stats**
   In Blueprint Details panel, scroll to "Character" section:
   - **Warrior**: STR=18, VIT=16, AGI=10, INT=6, DEX=10, WIS=8
   - **Mage**: INT=20, WIS=18, STR=6, VIT=8, AGI=8, DEX=10
   - **Rogue**: AGI=20, DEX=18, STR=12, VIT=8, INT=8, WIS=6
   - **Hunter**: DEX=20, AGI=16, INT=12, VIT=10, STR=10, WIS=8

3. **Assign 3D Character Mesh** (optional)
   - In Details → Search "Mesh"
   - Assign a skeletal mesh
   - Add animations via Animation Blueprint

4. **Test in PIE**
   - Press `Alt+P`
   - Create character and verify stats display correctly

### Blueprint Skills & Combat

1. **Create Skill Data Table**
   - Content Browser → New → Data Table
   - Row Structure: `FSkillData` (from GameDataTypes.h)
   - Add rows: Cleave, Fireball, Backstab, Void Shot, etc.

2. **Link to Character Blueprint**
   - Select `BP_Warrior` or class blueprint
   - Details → Assign data table reference

3. **Test Combat**
   - Play in editor
   - Create character → travel to PvP zone → engage enemy
   - Verify skills appear in combat UI

### Blueprint Zones & Enemies

1. **Create Zone Level**
   - File → New Level → Blank Level
   - Save as `MAP_Ironhaven`, `MAP_AshenWastes`, etc.

2. **Add Spawners**
   - Create Blueprint based on `AEnemySpawner`
   - Place in level
   - Configure: "Zone Type", "Max Active Enemies", "Spawn Interval"

3. **Add Environment**
   - Drag static meshes, foliage, lighting
   - Place a `PlayerStart` actor (where player spawns)
   - Set GameMode to `MMOGameMode`

4. **Test Zone Travel**
   - Play in editor
   - Travel to new zone from world map
   - Verify enemies spawn and combat works

## Running the Game

### Single Player (PIE - Play in Editor)
```
Press ALT + P or click Play button
- Test character creation
- Test combat mechanics
- Test zone travel

OR

Press ALT + Shift + P (Standalone, higher performance)
```

### Multiplayer (Local Network Test)
```
1. Edit → Project Settings → Engine → Multiplayer
   - Set "Number of Players" = 2
   
2. File → Package Project → Windows (64-bit)
   - Choose output folder
   
3. Run first instance (becomes server):
   Binaries/Win64/EchoesMMO.exe
   
4. Run second instance in another window (becomes client):
   Binaries/Win64/EchoesMMO.exe -join 127.0.0.1
   
5. Both players share the same world state
```

## ✅ Verification Checklist

After opening the editor, verify:

- [ ] Content Browser shows folders (Source/, Content/, etc.)
- [ ] Output Log has no fatal errors (warnings are OK)
- [ ] Can press `Alt+P` to play
- [ ] Character creation screen appears
- [ ] Can select class, faction, name
- [ ] Game loads to main world with HUD
- [ ] Can click zones on map
- [ ] Can enter combat with enemies
- [ ] Skills appear in combat UI
- [ ] Combat damage calculated correctly
- [ ] Can level up and see stats increase
- [ ] Zone travel works with level requirement check

**If any check fails**, see "Troubleshooting" section below.

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
- [x] Enemy spawning & AI
- [x] Loot generation system
- [x] Server-authoritative architecture

### ⏳ In Progress (Content Creation)
- [ ] Create Blueprint character variants (BP_Warrior, BP_Mage, etc.)
- [ ] Build animation sets (walk, attack, spell cast)
- [ ] Design zone levels with environments
- [ ] Polish UI/UX with artwork

### 📋 Next Phase
- [ ] NPC dialogue system
- [ ] Quest log & tracker
- [ ] PvP matchmaking
- [ ] Guild system with permissions
- [ ] Marketplace/Trading
- [ ] Seasonal events & battle pass

---

## 🐛 Troubleshooting

### Build Issues

**Q: "Module not found" error during compile**
```
A: Make sure EchoesMMO.Build.cs has all dependencies:
   "Core", "Engine", "UMG", "Slate", "OnlineSubsystem"
   
   Then:
   Clean → Clean Solution (Visual Studio)
   Right-click .uproject → Generate VS files again
   Rebuild
```

**Q: ".generated.h not found" error**
```
A: Delete Intermediate/ and Binaries/ folders
   Close Visual Studio
   Right-click EchoesMMO.uproject → Generate VS files
   Reopen and rebuild
```

**Q: Compilation takes 10+ minutes**
```
A: First build is slow (compiling engine headers).
   Subsequent builds are much faster (incremental).
   
   If consistently slow:
   - Disable Live Coding (Visual Studio options)
   - Use "Development" config instead of "DebugGame"
   - Ensure VS is on same drive as UE5
```

### Runtime Issues

**Q: Character doesn't spawn in game**
```
A: Check that your level has:
   1. A PlayerStart actor (drag into level)
   2. GameMode set to MMOGameMode
      (Project Settings → Maps & Modes → Default GameMode)
   3. No blocking geometry at spawn
```

**Q: Combat UI doesn't appear**
```
A: Verify MainHUD.cpp calls:
   CreateWidget<UCombatWidget>(...)->AddToViewport();
   
   And that Widget blueprint exists in Content/UI/
   Check Output Log for widget creation errors
```

**Q: Networking broken in 2-player test**
```
A: Verify both instances connect correctly:
   Client: EchoesMMO.exe -join 127.0.0.1
   
   Check firewall isn't blocking traffic
   
   Enable network logging:
   Edit → Project Settings → Engine → Networking
   Enable "Net Debug"
   
   Check Output Log for replication errors
```

**Q: Replication not syncing between players**
```
A: Check NETWORKING.md section "Replicated Data"
   Verify GetLifetimeReplicatedProps() in AMMOCharacter
   Add logging to verify RPC calls:
   
   UE_LOG(LogMMO, Warning, TEXT("Replicating stats"));
```

### Performance

**Q: Game runs slow in PIE**
```
A: Use Standalone mode instead (higher perf):
   Alt+Shift+P
   
   Or enable GPU profiling:
   Ctrl+Shift+Comma → Viewport Options → Enable Stats
```

**Q: Crashes at startup**
```
A: Check Saved/Logs/ folder for crash dump
   Open Output Log (Windows → Developer Tools → Output Log)
   Look for the error before crash
   
   Common causes:
   - Missing asset in blueprint
   - Invalid player controller reference
   - Corrupt save game file (delete Saved/SaveGames/)
```

### IntelliSense Issues (VS Code)

**Q: "Cannot find include" squiggles in code**
```
A: This is normal with UE projects in VS Code.
   IntelliSense is limited by design.
   Code still compiles fine.
   
   For full IntelliSense, use Visual Studio instead:
   - Open EchoesMMO.sln
   - Get full support
```

## 📚 Next Steps

1. Read [FILE_DIRECTORY.md](FILE_DIRECTORY.md) for complete code structure
2. Read [NETWORKING.md](NETWORKING.md) for multiplayer details
3. Read [GAMEPLAY_FLOW.md](GAMEPLAY_FLOW.md) for game design & features
4. Create Blueprint variants for each character class
5. Design your first level with enemies and NPCs
6. Test multiplayer with 2+ players

---

**Still stuck?** Check:
1. Output Log for error messages (most helpful)
2. Saved/Logs/ folder for detailed logs
3. [DELIVERY_SUMMARY.md](DELIVERY_SUMMARY.md) for feature overview
4. Engine documentation: https://docs.unrealengine.com/
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
