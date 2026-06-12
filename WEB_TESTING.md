# Automated Testing Guide (Jest)

The Web Prototype includes a comprehensive automated test suite (`echoes_mmo.test.js`) designed to run in a Node.js environment using Jest and JSDOM. It validates initial states, UI interactions, and the Save/Load mechanics.

## 🛠️ Setup Instructions

To run the tests, you must have Node.js installed.

1. **Initialize the project** (if you haven't already):
   ```bash
   npm init -y
   ```
2. **Install Jest and JSDOM**:
   ```bash
   npm install --save-dev jest jest-environment-jsdom
   ```
3. **Configure `package.json`**:
   Ensure your `package.json` has the test script set up correctly:
   ```json
   {
     "scripts": {
       "test": "jest --env=jsdom"
     }
   }
   ```

## 🏃 Running the Tests

Execute the following command in the terminal:
```bash
npm test
```

## 🧪 What is being tested?

1.  **Initial Screen State:** Ensures that upon loading `echoes_mmo.html`, only the Title Screen is visible, while Character Creation, Game Screens, and Combat Frames remain strictly hidden.
2.  **Title & Menus:** Validates text content, branding, and standard UI button generation.
3.  **HUD Initialization:** Checks that the internal default state (e.g., Level 1 Warrior, 1,250 Gold) is accurately represented in the DOM.
4.  **Save/Load Mechanics Sandbox:** The test suite reads `script.js` directly from the filesystem, wraps it in an isolated function block via `new Function()`, and mocks global dependencies (like DOMContentLoaded listeners). 
    - It injects arbitrary data into `gameState`.
    - Tests `saveGame()` to ensure `localStorage` is updated correctly.
    - Tests `loadGame()` with mocked save data and corrupted JSON formats to guarantee crash resilience.