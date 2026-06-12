const fs = require('fs');
const path = require('path');

// Before running tests, you need to set up a testing environment like Jest.
// 1. Install dependencies: `npm install --save-dev jest jest-environment-jsdom`
// 2. Ensure your package.json has a "test": "jest" script.
// 3. Run tests with `npm test`.

// Load the HTML file's content to test against
const html = fs.readFileSync(path.resolve(__dirname, './echoes_mmo.html'), 'utf8');

describe('Echoes MMO UI Tests', () => {

  beforeEach(() => {
    // Set up a new DOM environment for each test to ensure they are isolated
    document.body.innerHTML = html;
  });

  describe('Initial Screen State', () => {
    it('should display the title screen by default', () => {
      const titleScreen = document.getElementById('title-screen');
      // The 'hidden' class should not be present on the title screen
      expect(titleScreen.classList.contains('hidden')).toBe(false);
    });

    it('should hide the character creation screen by default', () => {
      const charScreen = document.getElementById('char-screen');
      expect(charScreen.classList.contains('hidden')).toBe(true);
    });

    it('should hide the main game screen by default', () => {
      const gameScreen = document.getElementById('game-screen');
      expect(gameScreen.classList.contains('hidden')).toBe(true);
    });
  });

  describe('Title Screen', () => {
    it('should display the main title and subtitle', () => {
      const title = document.querySelector('#title-screen h1');
      const subtitle = document.querySelector('#title-screen .sub');
      expect(title.textContent).toBe('Echoes');
      expect(subtitle.textContent).toBe('of the Fractured Realm');
    });

    it('should have four main menu buttons', () => {
      const buttons = document.querySelectorAll('#title-screen .title-btn');
      expect(buttons.length).toBe(4);
      expect(buttons[0].textContent).toBe('⚔ Enter the Realm');
      expect(buttons[1].textContent).toBe('◉ Select Server');
    });

    it('should have an onclick handler to enter the realm', () => {
        const enterButton = document.querySelector('.title-btn.primary');
        expect(enterButton.getAttribute('onclick')).toBe('showCharScreen()');
    });
  });

  describe('Character Creation Screen', () => {
    it('should contain the header and an input for character name', () => {
      const header = document.querySelector('#char-screen h2');
      const nameInput = document.getElementById('char-name-input');
      expect(header.textContent).toBe('Choose Your Legend');
      expect(nameInput).not.toBeNull();
      expect(nameInput.getAttribute('placeholder')).toBe('Enter your name...');
    });

    it('should have a "Begin Your Legend" button with an onclick handler', () => {
      const createBtn = document.querySelector('#char-screen .create-btn');
      expect(createBtn).not.toBeNull();
      expect(createBtn.textContent).toBe('Begin Your Legend →');
      expect(createBtn.getAttribute('onclick')).toBe('startGame()');
    });
  });

  describe('Main Game Screen HUD', () => {
    it('should have the top HUD with default player info', () => {
      const playerName = document.getElementById('hud-name');
      const playerClass = document.getElementById('hud-class');
      expect(playerName.textContent).toBe('Hero');
      expect(playerClass.textContent).toBe('Warrior · Lv.1');
    });

    it('should contain HP, MP, and XP bars', () => {
      expect(document.getElementById('hp-bar')).not.toBeNull();
      expect(document.getElementById('mp-bar')).not.toBeNull();
      expect(document.getElementById('xp-bar')).not.toBeNull();
    });

    it('should display initial stat values', () => {
        const gold = document.getElementById('hud-gold');
        const kills = document.getElementById('hud-kills');
        const zone = document.getElementById('hud-zone');
        expect(gold.textContent).toBe('1,250');
        expect(kills.textContent).toBe('0');
        expect(zone.textContent).toBe('Ironhaven');
    });

    it('should have 5 main navigation tabs with "World" as active', () => {
        const navButtons = document.querySelectorAll('.hud-nav .nav-btn');
        const worldTab = document.getElementById('tab-world');
        const pvpTab = document.getElementById('tab-pvp');

        expect(navButtons.length).toBe(5);
        expect(worldTab.textContent).toContain('World');
        expect(worldTab.classList.contains('active')).toBe(true);
        expect(pvpTab.classList.contains('active')).toBe(false);
    });
  });

  describe('Combat Overlay', () => {
    it('should be present but the main frame should be hidden initially', () => {
        const combatOverlay = document.getElementById('combat-overlay');
        const combatFrame = document.getElementById('combat-frame');
        expect(combatOverlay).not.toBeNull();
        expect(combatFrame.classList.contains('hidden')).toBe(true);
    });

    it('should contain player and enemy combat frames', () => {
        const playerFrame = document.getElementById('player-combat-info');
        const enemyFrame = document.getElementById('enemy-combat-info');
        expect(playerFrame).not.toBeNull();
        expect(enemyFrame).not.toBeNull();
    });

    it('should have victory and defeat screens that are initially hidden', () => {
        const victoryScreen = document.getElementById('victory-screen');
        const defeatScreen = document.getElementById('defeat-screen');
        expect(victoryScreen.classList.contains('hidden')).toBe(true);
        expect(defeatScreen.classList.contains('hidden')).toBe(true);
    });
  });

});