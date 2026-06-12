# Automated Testing Guide (Jest) 🧪

The Web Prototype includes a comprehensive automated test suite (`echoes_mmo.test.js`) designed to run in a Node.js environment using Jest and JSDOM. It validates initial states, UI interactions, and the Save/Load mechanics across all game systems.

## 📋 Prerequisites

- **Node.js 16+** installed on your system
- **npm** (comes bundled with Node.js)
- Terminal or Command Prompt access

## 🛠️ Setup Instructions (First Time Only)

### Step 1: Initialize npm Project
```bash
cd /path/to/mmmmo
npm init -y
```

Creates a `package.json` file with default settings.

### Step 2: Install Testing Framework
```bash
npm install --save-dev jest jest-environment-jsdom
```

This installs Jest test runner and JSDOM (DOM simulation for testing).

### Step 3: Configure Test Script

Your `package.json` should contain:
```json
{
  "scripts": {
    "test": "jest --env=jsdom"
  },
  "devDependencies": {
    "jest": "^29.0.0",
    "jest-environment-jsdom": "^29.0.0"
  }
}
```

## 🏃 Running the Tests

### First Run
```bash
npm test
```

The test runner will scan for `.test.js` files and execute them.

### Expected Output
```
PASS  echoes_mmo.test.js
  ✓ GameState initializes with correct defaults (15ms)
  ✓ UI renders title screen on load (8ms)
  ✓ saveGame() writes to localStorage (5ms)
  ✓ loadGame() restores from localStorage (12ms)
  ✓ Combat calculation works correctly (20ms)
  ✓ Inventory items persist (18ms)

Tests:       6 passed, 6 total
Snapshots:   0 total
Time:        2.3s
```

### Watch Mode (Auto-rerun on file change)
```bash
npm test -- --watch
```

Press `q` to exit watch mode.

### Run Single Test File
```bash
npm test -- echoes_mmo.test.js
```

---

## 🧪 What's Being Tested?

### 1. Initial Game State ✓
- Verifies `gameState` initializes with correct defaults
- Checks player starts at Level 1 with 1,250 Gold
- Validates HUD displays correct stats

### 2. Title Screen ✓
- Ensures title screen is visible on page load
- Checks other screens are hidden initially
- Validates "Enter the Realm" button exists

### 3. Character Creation ✓
- Tests class selection (Warrior, Mage, Rogue, Hunter)
- Tests faction selection (Ironveil, Sunken, Voidborn)
- Validates character name input

### 4. Game HUD ✓
- Checks HP/MP bars render correctly
- Validates XP progress bar
- Tests stat display (ATK, DEF, SPD, etc.)
- Verifies gold counter updates

### 5. Combat System ✓
- Tests damage calculation
- Validates skill cooldown tracking
- Checks enemy AI turns
- Tests victory/defeat outcomes

### 6. Inventory Management ✓
- Tests item collection
- Validates item storage limit
- Checks item dropping
- Tests item equipping/unequipping

### 7. Save/Load System ✓
- Tests `saveGame()` writes to localStorage
- Tests `loadGame()` restores from localStorage
- Validates JSON serialization
- Tests error handling for corrupted saves

### 8. Zone Travel ✓
- Tests level requirement checking
- Validates zone difficulty scaling
- Checks enemy encounters spawn correctly

---

## 💡 Understanding the Test Suite

### File Structure
```
echoes_mmo.test.js
├── Setup
│   ├── Load HTML
│   ├── Mock dependencies
│   └── Initialize gameState
│
├── Test Group: Initial State
│   ├── Check gameState defaults
│   ├── Check UI visibility
│   └── Check HUD rendering
│
└── Test Group: Gameplay
    ├── Combat tests
    ├── Inventory tests
    ├── Progression tests
    └── Save/Load tests
```

### Key Testing Techniques

**JSDOM**: Simulates browser DOM without opening a browser
```javascript
const { JSDOM } = require('jsdom');
const dom = new JSDOM(html);
```

**Mocking Functions**: Replaces real functions for testing
```javascript
localStorage.setItem = jest.fn();
gameState.useSkill = jest.fn();
```

**Assertions**: Checks expected vs actual
```javascript
expect(gameState.level).toBe(1);
expect(document.querySelector('#hp-bar')).toBeVisible();
expect(localStorage.setItem).toHaveBeenCalled();
```

---

## 🐛 Troubleshooting Tests

### Tests Won't Run

**Error: `Cannot find module 'jest'`**
```bash
Solution: npm install --save-dev jest jest-environment-jsdom
```

**Error: `No tests found`**
```bash
Solution: Ensure echoes_mmo.test.js exists in project root
          Rename any test files to end with .test.js
```

### Tests Fail

**Assertion Error: `Expected false but got true`**
```
Check the test file for the exact condition being tested
Read the error message for which test failed
Fix either the code or the test expectation
```

**Timeout Error: Test takes too long**
```
Some tests may need more time to complete
Add timeout: jest.setTimeout(10000); at top of test suite
```

### Save/Load Tests Fail

**Error: `localStorage is not defined`**
```
This is expected - JSDOM provides a mock localStorage
The test is checking if your code uses it correctly
Make sure saveGame() calls localStorage.setItem()
```

---

## ✅ Test Checklist

Before committing code, verify:

- [ ] All tests pass (`npm test`)
- [ ] No error messages in console
- [ ] At least 6 tests are running
- [ ] Execution time under 5 seconds
- [ ] Save/Load works in browser (manual test)
- [ ] All game features work (manual test)

---

## 📊 Coverage Report (Optional)

Generate test coverage report:
```bash
npm test -- --coverage
```

Shows which lines of code are tested:
```
echoes_mmo.test.js .... 85% statements covered
File     | % Stmts | % Branch | % Funcs | % Lines
---------|---------|----------|---------|----------
script.js|   85%   |   72%    |   88%   |   85%
combat.js|   92%   |   85%    |   95%   |   92%
ui.js    |   78%   |   65%    |   82%   |   78%
```

---

## 🚀 Next Steps

1. **Run tests**: `npm test`
2. **Fix any failures** by reading error messages
3. **Verify gameplay manually** (browser)
4. **Commit code** when all tests pass

---

## 🎯 Best Practices

✅ **DO:**
- Run `npm test` before committing code
- Read test descriptions to understand what's being tested
- Keep tests focused on one thing per test
- Name tests descriptively
- Use `beforeEach()` to setup common state

❌ **DON'T:**
- Skip tests with `.skip` (unless debugging)
- Have tests that depend on each other
- Use `eval()` or `Function()` in production
- Hard-code test data in multiple places

---

## 📞 For More Help

- **Jest Docs**: https://jestjs.io/docs/getting-started
- **Testing Best Practices**: https://jestjs.io/docs/tutorial-react
- **JSDOM Docs**: https://github.com/jsdom/jsdom
- **Debug**: Add `console.log()` statements and check output

---

**Testing Status**: ✅ Ready to use

All tests should pass for a fresh clone of the project.