# Complete Echoes MMO Gameplay Flow

## Game State Flow

```
┌─────────────────────────────────────────────────────────────┐
│  TITLE SCREEN                                               │
│  - Starfield animation                                      │
│  - "Enter the Realm" button                                 │
│  - Settings, Server Select, About                           │
└──────────────────┬──────────────────────────────────────────┘
                   │ [Player clicks "Enter the Realm"]
                   ▼
┌─────────────────────────────────────────────────────────────┐
│  CHARACTER CREATION                                         │
│  ┌─ Class Selection ─────────────────────────────────────┐ │
│  │ • Warbringer (⚔ Warrior)   - Tank/DPS                │ │
│  │   STR 18, VIT 16, HP 1400, Cleave, Shield Wall, etc │ │
│  │ • Arcanist (🔮 Mage)       - Burst DPS              │ │
│  │   INT 20, WIS 18, MP 900, Arcane Bolt, Mana Shield │ │
│  │ • Shadowblade (🗡 Rogue)   - Assassin               │ │
│  │   AGI 20, DEX 18, HP 900, Backstab, Poison Blade   │ │
│  │ • Voidhunter (🏹 Hunter)   - Ranged DPS            │ │
│  │   DEX 20, AGI 16, HP 950, Void Shot, Rain of Void  │ │
│  └──────────────────────────────────────────────────────┘ │
│  ┌─ Faction Selection ───────────────────────────────────┐ │
│  │ • Ironveil Dominion (⚙)  - +15% phys dmg, +2 slots  │ │
│  │ • Sunken Court (🌊)       - +20% magic dmg, breathing│ │
│  │ • Voidborn Conclave (🌀)  - +10% all stats, bonus fx│ │
│  └──────────────────────────────────────────────────────┘ │
│  ┌─ Name Your Champion ──────────────────────────────────┐ │
│  │ [Text Input] "Enter your name..."                    │ │
│  └──────────────────────────────────────────────────────┘ │
│  [Begin Your Legend →]                                     │
└──────────────────┬──────────────────────────────────────────┘
                   │ [Character created on server]
                   ▼
┌─────────────────────────────────────────────────────────────┐
│  MAIN GAME - WORLD EXPLORATION                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │ TOP HUD: Player Info Bar                             │  │
│  │ ⚡ Echoes │ [Avatar ⚔] Hero Warrior Lv.1            │  │
│  │ HP: ████████ 1000/1000  MP: ██████ 500/500          │  │
│  │ XP: ░░░░░░░░ 0/100                                  │  │
│  │ Gold: 1,250 | Kills: 0 | Rating: 1200 | Zone: Ironhaven
│  │ [🗺 World] [⚔ PvP] [⚑ Guild] [⚖ Market] [◈ Char]   │  │
│  │ ◉ Safe Zone | Eternal-1 | 2,847 Online              │  │
│  └──────────────────────────────────────────────────────┘  │
│                                                              │
│  ┌─ WORLD MAP (Canvas)  ──────────────────────────────┐   │
│  │ [Visual 2D world map with zones]                   │   │
│  │                                                    │   │
│  │  🏰 Ironhaven City (start)                         │   │
│  │  ☠ Ashen Wastes (PvP)                             │   │
│  │  🌲 Silvermere Forest (PvE)                        │   │
│  │  🌀 Void Rift (Elite PvP)                          │   │
│  │  ❄ Glacial Peaks (Dungeon)                        │   │
│  │  🌊 Sunken Temple (Raid)                           │   │
│  │  ⚔ War Front (Faction War)                         │   │
│  │                                                    │   │
│  │ [Zone Tooltip on hover]                            │   │
│  │ 🌲 Silvermere Forest                              │   │
│  │ Level: 5-20 | Type: PvE | Players: 12             │   │
│  │ "Ancient forest with arcane beasts"               │   │
│  │ [⚡ Travel Here]                                   │   │
│  └─ Minimap (bottom-right) ─────────────────────────┘   │
│  └─ World Controls: [+] [-] [⌂]                          │
│                                                              │
│  ┌─ RIGHT PANEL ─────────────────────────────────────┐   │
│  │ [Log] [Ranks]                                      │   │
│  │ ⚡ You entered the Realm                          │   │
│  │ 📍 You are in Ironhaven City (Safe Zone)          │   │
│  │ ⚔ Seek enemies in PvP zones to earn glory        │   │
│  │ (Leaderboard shows top 7 PvP players)            │   │
│  └────────────────────────────────────────────────────┘   │
└──────────────────┬──────────────────────────────────────────┘
                   │ [Player enters PvP zone]
                   ▼
┌─────────────────────────────────────────────────────────────┐
│  COMBAT - TURN-BASED ENCOUNTER                              │
│  ┌───────────────────────────────────────────────────────┐  │
│  │ COMBAT OVERLAY                                        │  │
│  │  ┌─────── PLAYER SIDE ──────┬─── ROUND 1 ───┬──── ENEMY ────┐
│  │  │ Hero                      │               │ Ash Marauder  │
│  │  │ Warbringer Lv.1          │               │ Enemy Lv.5    │
│  │  │ Buffs: —                 │               │ Debuffs: —    │
│  │  │ ████████ 1000/1000 HP   │               │ ████████ 450/600
│  │  │ ████ 450/500 MP          │               │               │
│  │  └───────────────────────────┴───────────────┴────────────────┘
│  │
│  │  ┌──── COMBAT ARENA ─────────────────────────────┐
│  │  │                                               │
│  │  │         ⚔    VS    👹                        │
│  │  │     Player          Enemy                   │
│  │  │                                               │
│  │  │   [Damage: -87] ↗️  [Damage: -45] ↖️         │
│  │  │                                               │
│  │  │     3x COMBO! ⭐⭐⭐                           │
│  │  │                                               │
│  │  │   ⚡ Hero used Cleave for 87 damage!         │
│  │  │   👹 Ash Marauder attacked for 45 damage!   │
│  │  └───────────────────────────────────────────────┘
│  │
│  │  ┌─ TURN INTERFACE ──────────────────────────────┐
│  │  │ Choose your action:                           │
│  │  │                                               │
│  │  │ [⚡ Basic Attack] [Cleave 20MP] [Shield 30MP]│
│  │  │ [Warcry 25MP] [Execute 50MP]                │
│  │  │ [↩ Flee Combat]                             │
│  │  └───────────────────────────────────────────────┘
│  └───────────────────────────────────────────────────────┘
│
│  [Victory!]                  OR        [Defeated]
│  +40 XP                                You have fallen...
│  +25 Gold                              [-50 Gold]
│  Rewards: [Loot dropped]               [⟳ Respawn at Shrine]
│  [⚡ Continue]
└──────────────────┬──────────────────────────────────────────┘
                   │ [Combat ends]
                   ▼
┌─────────────────────────────────────────────────────────────┐
│  PROGRESSION & REWARDS                                      │
│  ┌──────────────────────────────────────────────────────┐  │
│  │ Experience: 10/100 → 40/100                         │  │
│  │ Gold: 1,250 → 1,275                                 │  │
│  │ Kills: 0 → 1                                        │  │
│  │ Rating: 1200 → 1205                                 │  │
│  │                                                      │  │
│  │ 🎉 Loot Drops:                                      │  │
│  │ • Health Potion (x2) [Common]                       │  │
│  │ • Mana Potion [Common]                              │  │
│  │ • Iron Sword [Uncommon]                             │  │
│  └──────────────────────────────────────────────────────┘  │
│                                                              │
│  [At Level Up]                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  🎉 LEVEL UP!                                        │  │
│  │  You are now Level 2                                │  │
│  │                                                      │  │
│  │  Stats Improved:                                    │  │
│  │  • HP: 1400 → 1568 (+12%)                          │  │
│  │  • MP: 300 → 336 (+12%)                            │  │
│  │  • ATK: 85 → 95 (+12%)                             │  │
│  │  • Skill Points: +1                                 │  │
│  └──────────────────────────────────────────────────────┘  │
└──────────────────┬──────────────────────────────────────────┘
                   │ [Loop: Explore, Fight, Level]
                   ▼
┌─────────────────────────────────────────────────────────────┐
│  ADVANCED FEATURES (Unlocked at higher levels)              │
│  • PvP Arenas (vs other players)                            │
│  • Guild System (create/join guilds)                        │
│  • Marketplace (buy/sell items)                             │
│  • Faction Warfare (control zones for rewards)              │
│  • Raids (10-player dungeons)                               │
│  • Leaderboards (ranked by rating)                          │
└─────────────────────────────────────────────────────────────┘
```

## Skill Systems by Class

### Warbringer (Warrior)
| Skill | Cost | Cooldown | Effect |
|-------|------|----------|--------|
| Cleave | 20 MP | 2s | 90-130 physical damage |
| Shield Wall | 30 MP | 4s | +40% DEF for 2 turns |
| Warcry | 25 MP | 5s | +25% ATK for 3 turns |
| Execute | 50 MP | 6s | 200-280 dmg if enemy <20% HP |

### Arcanist (Mage)
| Skill | Cost | Cooldown | Effect |
|-------|------|----------|--------|
| Arcane Bolt | 25 MP | 1s | 110-160 magic damage |
| Frost Nova | 40 MP | 4s | Stun enemy 1 turn + 70-90 dmg |
| Mana Shield | 35 MP | 5s | Absorb 30% next hit |
| Arcane Surge | 80 MP | 7s | 180-260 AoE magic damage |

### Shadowblade (Rogue)
| Skill | Cost | Cooldown | Effect |
|-------|------|----------|--------|
| Backstab | 20 MP | 2s | 130-180 dmg (bonus from stealth) |
| Poison Blade | 25 MP | 3s | 50 DoT/turn for 3 turns |
| Smoke Bomb | 30 MP | 4s | Dodge next attack |
| Death Mark | 40 MP | 6s | +250% crit on next hit |

### Voidhunter (Hunter)
| Skill | Cost | Cooldown | Effect |
|-------|------|----------|--------|
| Void Shot | 20 MP | 1s | 85-120 dmg (ignores some armor) |
| Temporal Trap | 35 MP | 5s | Stun 2 turns + DoT |
| Phase Arrow | 40 MP | 4s | 100-140 magic dmg (ignores defense) |
| Rain of Void | 60 MP | 6s | 5 hits × 40 dmg (random targets) |

## Zone Progression Guide

```
Level 1-5:   Ironhaven City (Safe) → Ashen Wastes (PvP Light)
Level 5-10:  Silvermere Forest (PvE) → Glacial Peaks (Dungeon)
Level 10-20: Ashen Wastes (PvP Medium) → War Front (Faction)
Level 20-50: Void Rift (Elite PvP) → Sunken Temple (Raid)
Level 50+:   All zones (Farming, PvP, Endgame)
```

## Multiplayer Interactions

### PvP Encounters
- **Safe Zones**: No PvP (Ironhaven, Silvermere, Glacial Peaks)
- **PvP Zones**: Full PvP enabled (Ashen Wastes, Void Rift, War Front)
- **Ranking**: Winners gain rating points, losers lose rating
- **Matchmaking**: Based on level and rating

### Guilds
- Create with 5+ members
- Shared guild bank
- Guild chat/voice
- Participate in guild wars
- Unlock guild perks

### Marketplace
- Buy/sell items from other players
- Auction house (list items for time)
- Price determined by rarity & demand
- 5% transaction fee (gold sink)

### Faction War
- 3 factions compete for zone control
- Daily objective: Capture & hold zones
- Rewards: Gold, XP, faction reputation
- Highest rated faction wins season

## Endgame Content

### Raid Dungeons (10-player)
- Sunken Temple (40+ recommended)
- Challenging boss encounters
- Legendary loot drops
- Weekly lockout

### PvP Arenas
- 1v1, 3v3, 5v5 formats
- Rating-based matchmaking
- Seasonal rewards
- Leaderboards

### Faction Warfare
- Control 50%+ of zones for bonuses
- Daily battles over objectives
- Prestige system
- Exclusive cosmetics

## Save System & Progression

- **Auto-save**: Server saves every 5 minutes
- **Data Persisted**: Character stats, inventory, achievements, position
- **Account-wide**: Can create multiple characters
- **Cloud Sync**: Progress syncs across devices
- **Backup**: Daily server backups

---

**Time to Realm**: ~5 hours to reach level 20, 50+ hours for endgame
