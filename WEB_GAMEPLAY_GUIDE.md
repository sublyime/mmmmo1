# Web Prototype Gameplay Guide

## 🗡️ Classes

*   **Warbringer (Warrior):** High HP, high defense. Excels at survival. Use *Shield Wall* before heavy enemy attacks and *Execute* when the enemy is low on HP.
*   **Arcanist (Mage):** Highest MP and Magic Damage, very fragile. *Mana Shield* is critical to survival. Rely on *Frost Nova* to stun enemies and bypass damage entirely.
*   **Shadowblade (Rogue):** Fast, high evasion. Uses *Poison Blade* to stack Damage over Time (DoT) and *Smoke Bomb* to evade lethal hits.
*   **Voidhunter (Hunter):** Ranged DPS relying on combos. *Temporal Trap* is an excellent opener to stun enemies while you set up *Rain of Void*.

## 🗺️ Navigation & Zones

The world map consists of multiple zones. You must click a zone to view its details, then click "Travel Here" to move.

**Safe Zones (Green):**
*   *Ironhaven City*, *Silvermere Forest*, *Glacial Peaks*
*   You will regenerate HP and MP automatically over time in these zones.

**PvP / Danger Zones (Red dashed borders):**
*   *Ashen Wastes*, *Void Rift*, *War Front*
*   Entering these zones stops HP/MP regeneration.
*   You are susceptible to random enemy encounters while idle (triggered by the game tick).

## ⚔️ Combat Flow

Combat is strict Turn-Based.
1.  **Initiative:** Decided by comparing Player Speed (`spd`) against Enemy Speed.
2.  **Action Phase:** Select a skill or Basic Attack. Skills cost MP and have Cooldowns.
3.  **Enemy Phase:** The enemy AI will attack, applying reductions from your active buffs (e.g., *Shield Wall*, *Mana Shield*).
4.  **End of Combat:** Winning grants Gold, XP, and Rating. Dying results in a 30% Gold penalty and returns you to Ironhaven with 50% HP.

## ⚖️ Economy & Market

The Auction House allows you to purchase powerful gear using the Gold you farm from encounters. Prices are fixed in the prototype. 
*   Filter items by rarity or type.
*   Ensure you have enough gold before purchasing; clicking "Buy" automatically deducts the funds.

## ⚑ Faction Wars

By contributing to the Faction War in the PvP tab, you increase your faction's dominance. The progress bars simulate a server-wide territory control battle between Ironveil, Sunken, and Voidborn.

## 🌟 Progression

Experience required to level up scales by a factor of `1.5x` per level.
When leveling up:
- Max HP increases by 12%
- Max MP increases by 12%
- Base Attack and Defense increase by 10%

*Tip:* You can fully heal instantly when you level up. If you are close to leveling, try to find an easy encounter before resting!