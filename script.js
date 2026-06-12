// ============================================================
// GAME STATE
// ============================================================
let gameState = {
  screen: 'title',
  player: {
    name:'Hero', cls:null, faction:null,
    level:1, xp:0, xpNext:100,
    hp:1000, hpMax:1000, mp:500, mpMax:500,
    atk:75, def:35, spd:10,
    gold:1250, kills:0, deaths:0, rating:1200,
    zone:'ironhaven', effects:[],
  },
  combat: {
    active:false, round:1, enemy:null,
    enemyHp:0, enemyHpMax:0,
    playerTurn:true, combo:0,
    cooldowns:{},
    queued:null, queueTimer:null,
  },
  world: { zoom:1, offsetX:0, offsetY:0, hoveredZone:null, selectedZone:null },
  rightTab:'log',
  navTab:'world',
  pvpQueue:null,
  combatLog:[],
  factionWar:{ ironveil:42, sunken:28, voidborn:30 },
  bounties:[
    { name:'VoidReaper', cls:'Shadowblade', icon:'🗡', reward:850 },
    { name:'BloodKing', cls:'Warbringer', icon:'⚔', reward:1200 },
    { name:'PhaseGhost', cls:'Voidhunter', icon:'🏹', reward:640 },
  ],
  ticks:0,
};

// ============================================================
// GAME CONTROLLER / MAIN LOGIC
// ============================================================

function selectClass(id){
  gameState.player.cls=id;
  buildClassGrid();
}

function selectFaction(id){
  gameState.player.faction=id;
  buildFactionGrid();
}

function startGame(){
  if(!gameState.player.cls){ notify('Please select a class!','danger'); return; }
  if(!gameState.player.faction){ notify('Please select a faction!','danger'); return; }
  const name=document.getElementById('char-name-input').value.trim();
  if(!name){ notify('Please enter your character name!','danger'); return; }
  const cls=CLASSES.find(c=>c.id===gameState.player.cls);
  const fac=FACTIONS.find(f=>f.id===gameState.player.faction);
  gameState.player.name=name;
  gameState.player.hpMax=cls.hp; gameState.player.hp=cls.hp;
  gameState.player.mpMax=cls.mp; gameState.player.mp=cls.mp;
  gameState.player.atk=cls.atk; gameState.player.def=cls.def; gameState.player.spd=cls.spd;
  document.getElementById('char-screen').classList.add('hidden');
  document.getElementById('game-screen').classList.remove('hidden');
  initGameUI();
  setTimeout(()=>{ notify(`Welcome, ${name}!`,'success'); notify(`${fac.name}: ${fac.bonus}`,'info'); },500);
  startGameTick();
}

function travelToZone(zoneId){
  const z=ZONES.find(z=>z.id===(zoneId||document.getElementById('travel-btn').dataset.zone));
  if(!z) return;
  if(z.id===gameState.player.zone){ notify(`Already in ${z.name}`,'info'); return; }
  addLog(`🧭 Traveling to ${z.name}...`,'system');
  notify(`Entered ${z.name}`,'info');
  gameState.player.zone=z.id;
  gameState.world.selectedZone=z.id;
  updateHUD(); drawWorld();
  if(z.pvp) setTimeout(()=>notify('⚔ Warning: You are in a PvP zone!','danger'),800);
  if(z.pvp) setTimeout(()=>startEncounter(),2000+Math.random()*3000);
}

function zoomWorld(factor){
  gameState.world.zoom=Math.max(0.5,Math.min(2.5,gameState.world.zoom*factor));
  drawWorld();
}
function resetZoom(){ gameState.world.zoom=1; gameState.world.offsetX=0; gameState.world.offsetY=0; drawWorld(); }

// ============================================================
// PVP QUEUE
// ============================================================
function toggleQueue(arenaId){
  if(gameState.pvpQueue===arenaId){
    gameState.pvpQueue=null;
    notify('Left queue.','info');
    renderRightPanel();
    return;
  }
  gameState.pvpQueue=arenaId;
  notify('Joined queue for '+arenaId+'!','info');
  renderRightPanel();
  const wait=3000+Math.floor(Math.random()*5000);
  setTimeout(()=>{
    if(gameState.pvpQueue===arenaId){
      gameState.pvpQueue=null;
      notify('⚔ Match Found!','success');
      addLog(`⚔ PvP Match starting: ${arenaId}!`,'pvp');
      renderRightPanel();
      setTimeout(()=>startEncounterFromArena('pvp'),500);
    }
  },wait);
}

// ============================================================
// LOGIC & STATE
// ============================================================

function addLog(msg,type){ gameState.combatLog.push({msg,type}); if(gameState.rightTab==='log') renderRightPanel(); }

function buyItem(name,price){
  if(gameState.player.gold<price){ notify('Not enough gold!','danger'); return; }
  gameState.player.gold-=price;
  notify(`Purchased: ${name}!`,'success');
  addLog(`⚖ Bought ${name} for ${price.toLocaleString()}G`,'system');
  updateHUD();
}

function switchRightTab(tab){
  gameState.rightTab=tab;
  document.querySelectorAll('.panel-tab').forEach(t=>t.classList.remove('active'));
  document.getElementById('rtab-'+tab).classList.add('active');
  renderRightPanel();
}

// ============================================================
// FACTION WAR
// ============================================================
function factionAttack(){
  const fac=gameState.player.faction;
  if(!fac){ notify('Join a faction first!','danger'); return; }
  const fw=gameState.factionWar;
  const gain=2+Math.floor(Math.random()*4);
  fw[fac]=Math.min(70,fw[fac]+gain);
  const total=fw.ironveil+fw.sunken+fw.voidborn;
  const excess=total-100;
  const others=['ironveil','sunken','voidborn'].filter(f=>f!==fac);
  others[0]&&(fw[others[0]]=Math.max(5,fw[others[0]]-Math.ceil(excess/2)));
  others[1]&&(fw[others[1]]=Math.max(5,fw[others[1]]-Math.floor(excess/2)));
  notify(`⚔ Faction War: +${gain}% territory!`,'success');
  addLog(`⚑ ${FACTIONS.find(f=>f.id===fac).name} gains ${gain}% territory!`,'pvp');
  if(gameState.navTab==='pvp') showFullPanel('pvp');
}

// ============================================================
// GAME TICK — ambient events
// ============================================================
function startGameTick(){
  setInterval(()=>{
    gameState.ticks++;
    // Online count variation
    const base=2847+Math.floor(Math.sin(gameState.ticks*0.1)*200+Math.random()*50);
    document.getElementById('online-count').textContent=base.toLocaleString()+' Online';
    // Ambient PvP events in log
    if(gameState.ticks%8===0){
      const killers=['VoidReaper','IronCrown','ArcaneStorm','PhaseWalker','Deathweave'];
      const victims=['GreyHero','LostSoul','WandererX','FieldMage','QuickBlade'];
      const zones_pvp=['Ashen Wastes','Void Rift','War Front'];
      const k=killers[Math.floor(Math.random()*killers.length)];
      const v=victims[Math.floor(Math.random()*victims.length)];
      const z=zones_pvp[Math.floor(Math.random()*zones_pvp.length)];
      addLog(`⚔ [${z}] ${k} slew ${v}`,'pvp');
    }
    // HP regen if not in combat and not in pvp zone
    if(!gameState.combat.active){
      const zone=ZONES.find(z=>z.id===gameState.player.zone);
      if(!zone||!zone.pvp){
        const p=gameState.player;
        if(p.hp<p.hpMax) p.hp=Math.min(p.hpMax,p.hp+Math.floor(p.hpMax*0.02));
        if(p.mp<p.mpMax) p.mp=Math.min(p.mpMax,p.mp+Math.floor(p.mpMax*0.03));
        updateHUD();
      }
    }
    // Random ambient notify
    if(gameState.ticks%20===0){
      const events=[
        ['⚔ Server event: Double XP weekend begins!','info'],
        ['📢 New raid tier unlocked: Sunken Vault','purple'],
        ['⚑ Faction War: Voidborn surges to 35%!','danger'],
        ['💰 Market surge: Dragon Scales at 15,000G','info'],
      ];
      const ev=events[Math.floor(Math.random()*events.length)];
      notify(ev[0],ev[1]);
    }
    // Random PvP encounter in pvp zones
    if(gameState.ticks%15===0&&!gameState.combat.active){
      const zone=ZONES.find(z=>z.id===gameState.player.zone);
      if(zone&&zone.pvp&&Math.random()<0.35){ notify('⚔ Enemy spotted!','danger'); startEncounter(); }
    }
  },2000);
}

// ============================================================
// INIT
// ============================================================
buildStars();
