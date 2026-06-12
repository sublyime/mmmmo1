# Echoes of the Fractured Realm

Welcome to **Echoes of the Fractured Realm**, an ambitious dual-architecture MMO project. 

This repository contains two distinct implementations of the same game universe:
1. **The Web Prototype (`/`)**: A highly polished, browser-based prototype built with vanilla HTML, CSS, and JavaScript. It features a complete game loop, turn-based combat, mock multiplayer systems (chat, market), and local-storage save functionality.
2. **The Unreal Engine 5 Port (`/UnrealPort`)**: A production-ready C++ and UMG implementation transitioning the prototype into a fully replicated, 3D networked multiplayer environment.

---

## 📁 Repository Structure

```text
mmmmo/
├── echoes_mmo.html         # Main entry point for the web game
├── script.js               # Game state and main logic loop
├── ui.js                   # DOM manipulation, canvas drawing, UI state
├── combat.js               # Turn-based combat math and logic
├── data.js                 # Data tables (Classes, Zones, Factions, Enemies)
├── style.css               # Theming and layout for the browser version
├── echoes_mmo.test.js      # Jest testing suite for the web version
├── WEB_ARCHITECTURE.md     # Web prototype technical documentation
├── WEB_GAMEPLAY_GUIDE.md   # Web prototype player guide
├── WEB_TESTING.md          # Web prototype unit testing guide
│
└── UnrealPort/             # Unreal Engine 5 C++ Port
    ├── README.md           # UE5 specific documentation
    ├── FILE_DIRECTORY.md   # UE5 code structure
    ├── QUICK_START.md      # UE5 build instructions
    ├── NETWORKING.md       # UE5 replication documentation
    ├── GAMEPLAY_FLOW.md    # UE5 transition mechanics
    └── Source/             # C++ Source Code
```

---

## 🚀 Quick Start

### Web Prototype
To play the web version, simply open the `echoes_mmo.html` file in any modern web browser. No server or build process is required for local play. All save data is kept in your browser's `localStorage`.

### Unreal Engine 5 Port
Navigate to the `UnrealPort/` directory. You will need Unreal Engine 5.4+ and Visual Studio 2022. Generate the project files by right-clicking `EchoesMMO.uproject`, compile the solution, and open it in the UE5 Editor. See `UnrealPort/QUICK_START.md` for complete details.

---

## 📖 Documentation Index

*   **Web Architecture**: Deep dive into the JS/CSS structure.
*   **Web Gameplay**: How the mechanics work under the hood.
*   **Web Testing**: How to run the automated Jest DOM tests.
*   **UE5 Documentation**: The gateway to the C++ server-authoritative port docs.