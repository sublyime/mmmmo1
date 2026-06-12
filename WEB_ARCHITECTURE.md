# Web Prototype Architecture

The Web version of "Echoes of the Fractured Realm" is built purely in vanilla web technologies (HTML5, CSS3, ES6 JavaScript) to ensure rapid prototyping and extreme portability. It requires no backend server; multiplayer elements are "ambiently simulated" to feel alive while remaining a single-player offline experience.

## 🏗️ Core Philosophy

- **Zero Dependencies:** No React, Vue, or heavy frameworks. Pure DOM manipulation for maximum speed and simplicity.
- **Single Source of Truth:** The entire state is contained within the `gameState` object in `script.js`.
- **Event-Driven UI:** UI updates are triggered explicitly after state changes (via `updateHUD()`, `buildSkillBar()`, etc.).

---

## 📄 File Breakdown

### 1. `script.js` (The Brain)
Contains the core `gameState` object, which holds everything from player stats to current combat cooldowns and the faction war progress. 

**Key Functions:**
- `startGameTick()`: A `setInterval` loop that runs every 2 seconds. It simulates online user count fluctuations, handles ambient HP/MP regeneration, generates random server notifications, and triggers random PvP encounters.
- `saveGame()` & `loadGame()`: Serializes the `gameState` object into a JSON string and stores it in the browser's `localStorage`.
- Navigation controllers (`switchRightTab`, `travelToZone`).

### 2. `combat.js` (The Engine)
Handles all logic for the turn-based combat system.

**Key Systems:**
- `startEncounter()`: Initializes combat, selects a random enemy based on the current zone, and scales enemy stats to the player's level.
- `useSkill()`: Deducts MP, calculates damage, handles physical/magic type resistances, and applies specific effects (DoTs, Stuns, Buffs).
- `enemyTurn()`: A lightweight AI that processes DoT ticks, checks for stuns, and attacks the player (accounting for player evasion and defensive buffs).
- `endCombat()`: Distributes rewards (XP, Gold, Rating), checks for level-ups, and handles respawn logic if the player dies.

### 3. `ui.js` (The View)
Manages DOM manipulation and the HTML5 Canvas maps.

**Key Systems:**
- **String Interpolation Rendering:** Uses template literals to dynamically inject HTML (e.g., `buildPvPPanel()`, `buildMarketPanel()`).
- **Canvas World Map (`drawWorld`)**: Dynamically draws the world map onto an HTML5 `<canvas>`, including zooming, panning, and rendering zones based on X/Y coordinates defined in `data.js`.
- **Combat Overlay:** Manipulates CSS classes to trigger attack (`attack-anim`) and hurt (`hurt-anim`) animations.

### 4. `data.js` (The Database)
Stores constant JSON arrays that populate the game universe.

- `CLASSES`: Defines Warbringer, Arcanist, Shadowblade, Voidhunter (base stats, skills).
- `FACTIONS`: Ironveil, Sunken, Voidborn (passive bonuses).
- `ZONES`: Determines map position, safety/PvP flags, and level brackets.
- `ENEMIES`: Base enemy templates that are scaled during encounters.

### 5. `style.css` (The Theme)
A comprehensive stylesheet heavily utilizing CSS Variables (`:root`) for easy theming.
- **Color Palette**: Dark themes (`--bg-deep`) with Gold (`--gold`) and distinct class/faction colors.
- **Animations**: Includes CSS keyframes for floating damage numbers (`floatDmg`), leveling up (`levelUp`), and notification slides (`slideIn`).

---

## 💾 Save System

The game autosaves when the window is closed via the `beforeunload` event. 
```javascript
window.addEventListener('beforeunload', saveGame);
```
When the game loads, it checks `localStorage.getItem('echoes_save')`. If found, it parses the JSON directly back into the `gameState` object and initializes the UI to bypass the title screen. Active combats are automatically dismissed on load to prevent UI desynchronization.