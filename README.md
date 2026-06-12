# Echoes of the Fractured Realm 🎮

Welcome to **Echoes of the Fractured Realm**, an ambitious dual-architecture MMO project showcasing modern game development across both web and engine-based platforms.

This repository contains two distinct implementations of the same game universe:
1. **The Web Prototype (`/`)**: A highly polished, browser-based prototype built with vanilla HTML, CSS, and JavaScript. It features a complete game loop, turn-based combat, mock multiplayer systems (chat, market), and local-storage save functionality.
2. **The Unreal Engine 5 Port (`/UnrealPort`)**: A production-ready C++ and UMG implementation transitioning the prototype into a fully replicated, 3D networked multiplayer environment with server-authoritative architecture.

---

## 🛠️ Development Environment Setup

### System Requirements
- **Web Version**: Any modern web browser (Chrome, Firefox, Safari, Edge) | Node.js 16+ (for testing)
- **UE5 Version**: Windows 10/11 or Linux | Unreal Engine 5.4+ | Visual Studio 2022 (C++ workload) | 100GB+ free disk space

### Quick Setup

#### Web Development
```bash
# Install test dependencies
npm install --save-dev jest jest-environment-jsdom

# Run tests
npm test

# Play locally
open echoes_mmo.html  # macOS
start echoes_mmo.html # Windows
```

#### UE5 Development (VS Code C++ IntelliSense)
```bash
cd UnrealPort

# Windows: Right-click .uproject → Generate Visual Studio project files
# Linux: ./GenerateProjectFiles.sh

# Build & open
open EchoesMMO.sln  # Visual Studio
# Then open EchoesMMO.uproject in UE5 Editor
```

**VS Code C++ Configuration** (Auto-configured in `.vscode/c_cpp_properties.json`):
- Scans Unreal Engine include paths
- Disables IntelliSense squiggles for compatibility
- Supports code navigation and basic completion

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

### Web Prototype (2 minutes)
1. Open `echoes_mmo.html` in your browser
2. Create a character and start exploring
3. Optionally run tests: `npm test`

**No server required.** All data is saved locally to `localStorage`.

### Unreal Engine 5 Port (10 minutes)
1. Install **Unreal Engine 5.4+** and **Visual Studio 2022**
2. Navigate to `UnrealPort/` directory
3. Right-click `EchoesMMO.uproject` → "Generate Visual Studio project files"
4. Open `EchoesMMO.sln` in Visual Studio → Build
5. Open `EchoesMMO.uproject` in UE5 Editor
6. See `UnrealPort/QUICK_START.md` for detailed walkthrough

---

## 📚 Documentation Index

### Web Version
| Document | Purpose |
|----------|----------|
| [WEB_ARCHITECTURE.md](WEB_ARCHITECTURE.md) | Technical deep-dive into JS/CSS structure and state management |
| [WEB_GAMEPLAY_GUIDE.md](WEB_GAMEPLAY_GUIDE.md) | Player guide: classes, zones, combat, progression |
| [WEB_TESTING.md](WEB_TESTING.md) | Jest testing setup and automated test suite guide |

### Unreal Engine 5 Port
| Document | Purpose |
|----------|----------|
| [UnrealPort/README.md](UnrealPort/README.md) | Architecture overview, core systems, C++ structure |
| [UnrealPort/QUICK_START.md](UnrealPort/QUICK_START.md) | Setup, Blueprint creation, testing checklist, VS Code config |
| [UnrealPort/FILE_DIRECTORY.md](UnrealPort/FILE_DIRECTORY.md) | Complete code structure and file organization |
| [UnrealPort/GAMEPLAY_FLOW.md](UnrealPort/GAMEPLAY_FLOW.md) | Complete game flow from title screen to endgame |
| [UnrealPort/NETWORKING.md](UnrealPort/NETWORKING.md) | Multiplayer architecture, replication, server setup |
| [UnrealPort/DELIVERY_SUMMARY.md](UnrealPort/DELIVERY_SUMMARY.md) | Feature checklist and implementation status |

---

## 🐛 Common Issues & Troubleshooting

### Web Version
**Q: Save data disappeared**  
A: Check browser Developer Tools → Application → LocalStorage. Try clearing cache and reloading.

**Q: Tests fail to run**  
A: Ensure `package.json` exists with `"test": "jest --env=jsdom"` script. Run `npm install`.

### UE5 Version
**Q: IntelliSense errors in VS Code**  
A: Auto-configured via `.vscode/c_cpp_properties.json`. Reload VS Code or rebuild with `ctrl+shift+p` → "Rebuild IntelliSense".

**Q: UE5 Editor won't open project**  
A: Regenerate project files: Right-click `.uproject` → Generate Visual Studio project. Then rebuild in Visual Studio.

**Q: Missing dependencies/compilation errors**  
A: Run `UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh` (Linux) or use Visual Studio on Windows.

See individual documentation files for additional troubleshooting.