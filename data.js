// ============================================================
// GAME DATA
// ============================================================
const CLASSES = [
  { id:'warrior', name:'Warbringer', role:'Tank / DPS', icon:'⚔', color:'#c0621a',
    desc:'Masters of brutal melee combat. Unbreakable defense with devastating counter-strikes.',
    stats:['STR 18','VIT 16','AGI 10','INT 6'],
    hp:1400, mp:300, atk:85, def:60, spd:8,
    skills:[
      {name:'Cleave',icon:'⚔',desc:'Wide arc strike',dmg:'90-130',cost:20,cd:2,type:'physical'},
      {name:'Shield Wall',icon:'🛡',desc:'+40% defense for 2 turns',dmg:'Buff',cost:30,cd:4,type:'buff'},
      {name:'Warcry',icon:'📣',desc:'Boost ATK 25% (3 turns)',dmg:'Buff',cost:25,cd:5,type:'buff'},
      {name:'Execute',icon:'💀',desc:'Triple dmg below 20% HP',dmg:'200-280',cost:50,cd:6,type:'physical'},
    ]},
  { id:'mage', name:'Arcanist', role:'Burst DPS', icon:'🔮', color:'#4a90d9',
    desc:'Wields volatile arcane energies. Devastating burst potential at the cost of fragility.',
    stats:['INT 20','WIS 18','STR 6','VIT 8'],
    hp:800, mp:900, atk:110, def:25, spd:7,
    skills:[
      {name:'Arcane Bolt',icon:'✨',desc:'Pure mana projectile',dmg:'110-160',cost:25,cd:1,type:'magic'},
      {name:'Frost Nova',icon:'❄',desc:'Freeze enemy 1 turn',dmg:'70-90',cost:40,cd:4,type:'magic'},
      {name:'Mana Shield',icon:'🔵',desc:'Absorb 30% next hit',dmg:'Shield',cost:35,cd:5,type:'buff'},
      {name:'Arcane Surge',icon:'🌀',desc:'Massive AoE blast',dmg:'180-260',cost:80,cd:7,type:'magic'},
    ]},
  { id:'rogue', name:'Shadowblade', role:'Assassin', icon:'🗡', color:'#6a2a8a',
    desc:'Lethal stealth operative. Combos and poison stacks melt enemies before they can react.',
    stats:['AGI 20','DEX 18','STR 12','VIT 8'],
    hp:900, mp:600, atk:100, def:30, spd:14,
    skills:[
      {name:'Backstab',icon:'🗡',desc:'High damage from stealth',dmg:'130-180',cost:20,cd:2,type:'physical'},
      {name:'Poison Blade',icon:'☠',desc:'50 dmg/turn for 3 turns',dmg:'DoT',cost:25,cd:3,type:'poison'},
      {name:'Smoke Bomb',icon:'💨',desc:'Evade next attack',dmg:'Dodge',cost:30,cd:4,type:'buff'},
      {name:'Death Mark',icon:'💀',desc:'Guaranteed crit next hit',dmg:'x2.5',cost:40,cd:6,type:'buff'},
    ]},
  { id:'hunter', name:'Voidhunter', role:'Ranged DPS', icon:'🏹', color:'#1a7a3a',
    desc:'Precision marksman who bends space-time. Kites enemies into devastating trap combos.',
    stats:['DEX 20','AGI 16','INT 12','VIT 10'],
    hp:950, mp:650, atk:95, def:35, spd:12,
    skills:[
      {name:'Void Shot',icon:'🏹',desc:'Tears through armor',dmg:'85-120',cost:20,cd:1,type:'physical'},
      {name:'Temporal Trap',icon:'⏳',desc:'Stun 2 turns + DoT',dmg:'Stun',cost:35,cd:5,type:'debuff'},
      {name:'Phase Arrow',icon:'🌌',desc:'Ignores all defense',dmg:'100-140',cost:40,cd:4,type:'magic'},
      {name:'Rain of Void',icon:'🌧',desc:'5 hits, random targets',dmg:'40×5',cost:60,cd:6,type:'physical'},
    ]},
];

const FACTIONS = [
  { id:'ironveil', name:'Ironveil Dominion', icon:'⚙', color:'#6a8fa8',
    desc:'Industrial empire of steel and innovation.',
    bonus:'Combat Mastery: +15% physical dmg, +2 Gear slots' },
  { id:'sunken', name:'The Sunken Court', icon:'🌊', color:'#8a6a2a',
    desc:'Ancient maritime civilization with forbidden magic.',
    bonus:'Arcane Depths: +20% magic dmg, Underwater breathing' },
  { id:'voidborn', name:'Voidborn Conclave', icon:'🌀', color:'#6a2a8a',
    desc:'Cult of interdimensional beings. Power beyond comprehension.',
    bonus:'Void Infusion: +10% all stats, Random bonus effect per kill' },
];

const ZONES = [
  { id:'ironhaven', name:'Ironhaven City', x:0.35, y:0.45, size:60, color:'#4a5060', type:'City', level:'All', desc:'The capital city. Trade, guilds, and politics.', pvp:false, icon:'🏰' },
  { id:'ashen_wastes', name:'Ashen Wastes', x:0.12, y:0.3, size:80, color:'#5a4030', type:'PvP Zone', level:'1–15', desc:'Contested territory. Outlaws and bounties thrive here.', pvp:true, icon:'☠' },
  { id:'silvermere', name:'Silvermere Forest', x:0.6, y:0.25, size:70, color:'#1a4a1a', type:'PvE', level:'5–20', desc:'Ancient forest teeming with Arcane beasts.', pvp:false, icon:'🌲' },
  { id:'void_rift', name:'The Void Rift', x:0.75, y:0.6, size:55, color:'#3a1a5a', type:'Elite PvP', level:'30–50', desc:'Dimensional fracture. Endgame PvP with legendary drops.', pvp:true, icon:'🌀' },
  { id:'glacial_peak', name:'Glacial Peaks', x:0.25, y:0.65, size:65, color:'#2a3a5a', type:'PvE Dungeon', level:'10–25', desc:'Frozen citadel housing the ice dragon Verathos.', pvp:false, icon:'❄' },
  { id:'sunken_temple', name:'Sunken Temple', x:0.55, y:0.72, size:50, color:'#1a3a4a', type:'Raid', level:'40–60', desc:'Ancient temple submerged in cursed waters. 10-player raid.', pvp:false, icon:'🌊' },
  { id:'faction_war', name:'War Front', x:0.42, y:0.2, size:55, color:'#5a1a1a', type:'Faction War', level:'20+', desc:'Daily faction war. Hold objectives to earn Honor.', pvp:true, icon:'⚔' },
];

const ENEMIES = [
  { name:'Ash Marauder', icon:'👹', hp:600, atk:65, def:20, spd:7, xp:40, gold:25, type:'pvp', level:5 },
  { name:'Shadow Wraith', icon:'👻', hp:450, atk:80, def:10, spd:12, xp:35, gold:20, type:'magic', level:4 },
  { name:'Iron Golem', icon:'🤖', hp:900, atk:55, def:55, spd:4, xp:60, gold:40, type:'tank', level:8 },
  { name:'Void Stalker', icon:'🌀', hp:700, atk:90, def:25, spd:10, xp:70, gold:50, type:'elite', level:12 },
  { name:'Frost Drake', icon:'🐉', hp:1200, atk:100, def:40, spd:9, xp:120, gold:80, type:'boss', level:20 },
  { name:'Corrupted Knight', icon:'🧟', hp:800, atk:75, def:45, spd:6, xp:80, gold:55, type:'pvp', level:10 },
  { name:'Arcane Sentinel', icon:'🔵', hp:550, atk:85, def:15, spd:11, xp:45, gold:30, type:'magic', level:6 },
];

const MARKET_ITEMS = [
  { name:"Doomhammer of Ages", icon:'🔨', rarity:'legendary', type:'Weapon', stats:'+85 ATK, Chance: Shatter', price:8500, qty:1 },
  { name:"Voidweave Cloak", icon:'🧥', rarity:'epic', type:'Armor', stats:'+40 DEF, +15 SPD, Shadow Step', price:2200, qty:3 },
  { name:"Elixir of Giants", icon:'⚗', rarity:'rare', type:'Consumable', stats:'+30% HP for 1hr', price:450, qty:12 },
  { name:"Shattered Crystal", icon:'💎', rarity:'epic', type:'Material', stats:'Crafting: Arcane gear', price:1800, qty:5 },
  { name:"Ironveil Plate", icon:'🛡', rarity:'rare', type:'Armor', stats:'+55 DEF, +20 VIT', price:950, qty:2 },
  { name:"Mana Potion (x5)", icon:'🔵', rarity:'uncommon', type:'Consumable', stats:'Restore 300 MP', price:180, qty:20 },
  { name:"Void Shard", icon:'🌀', rarity:'epic', type:'Material', stats:'Crafting: Void weapons', price:3200, qty:2 },
  { name:"Swift Boots", icon:'👢', rarity:'uncommon', type:'Armor', stats:'+25 SPD, +10 AGI', price:320, qty:4 },
  { name:"Dragon Scale", icon:'🐉', rarity:'legendary', type:'Material', stats:'Crafting: Dragon armor', price:12000, qty:1 },
  { name:"Health Potion (x10)", icon:'❤', rarity:'common', type:'Consumable', stats:'Restore 250 HP', price:90, qty:50 },
];

const LEADERBOARD_PVP = [
  { name:'VoidReaper', cls:'Shadowblade', score:4280, icon:'🗡', rank:1 },
  { name:'IronCrown', cls:'Warbringer', score:3915, icon:'⚔', rank:2 },
  { name:'ArcaneStorm', cls:'Arcanist', score:3740, icon:'🔮', rank:3 },
  { name:'PhaseWalker', cls:'Voidhunter', score:3520, icon:'🏹', rank:4 },
  { name:'Deathweave', cls:'Shadowblade', score:3210, icon:'🗡', rank:5 },
  { name:'TitanBane', cls:'Warbringer', score:2990, icon:'⚔', rank:6 },
  { name:'MysticFlame', cls:'Arcanist', score:2750, icon:'🔮', rank:7 },
];

const GUILD_MEMBERS = [
  { name:'TitanBane', status:'online', zone:'Void Rift', cls:'⚔' },
  { name:'ArcaneStorm', status:'online', zone:'Silvermere', cls:'🔮' },
  { name:'ShadowDancer', status:'away', zone:'Ironhaven', cls:'🗡' },
  { name:'PhaseWalker', status:'online', zone:'War Front', cls:'🏹' },
  { name:'IronFist', status:'away', zone:'Ironhaven', cls:'⚔' },
];