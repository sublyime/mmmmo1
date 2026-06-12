// ============================================================
// COMBAT SYSTEM
// ============================================================
function startEncounter(enemyOverride){
  if(gameState.combat.active) return;
  const zone=ZONES.find(z=>z.id===gameState.player.zone);
  const pool=enemyOverride?[enemyOverride]:ENEMIES.filter(e=>e.type==='pvp'||e.type==='elite');
  const enemy={...pool[Math.floor(Math.random()*pool.length)]};
  const lvlMult=1+(gameState.player.level-1)*0.15;
  enemy.hp=Math.floor(enemy.hp*lvlMult); enemy.atk=Math.floor(enemy.atk*lvlMult);
  gameState.combat.active=true;
  gameState.combat.enemy={...enemy};
  gameState.combat.enemyHp=enemy.hp;
  gameState.combat.enemyHpMax=enemy.hp;
  gameState.combat.round=1;
  gameState.combat.combo=0;
  gameState.combat.cooldowns={};
  gameState.combat.playerTurn=gameState.player.spd>=enemy.spd;
  const cls=CLASSES.find(c=>c.id===gameState.player.cls);
  document.getElementById('c-player-name').textContent=gameState.player.name;
  document.getElementById('c-player-class').textContent=cls.name+' · Lv.'+gameState.player.level;
  document.getElementById('c-enemy-name').textContent=enemy.name;
  document.getElementById('c-enemy-class').textContent='Enemy · Lv.'+enemy.level;
  document.getElementById('fighter-enemy').textContent=enemy.icon;
  document.getElementById('fighter-player').textContent=cls.icon;
  document.getElementById('victory-screen').classList.add('hidden');
  document.getElementById('defeat-screen').classList.add('hidden');
  document.getElementById('combat-frame').classList.remove('hidden');
  updateCombatUI();
  buildSkillBar();
  if(!gameState.combat.playerTurn){ setCombatStatus('Enemy acts first!'); setTimeout(enemyTurn,1200); }
  else setCombatStatus('Choose your action!');
  addLog(`⚔ Encounter: ${enemy.name} appears!`,'pvp');
}

function buildSkillBar(){
  const cls=CLASSES.find(c=>c.id===gameState.player.cls);
  const bar=document.getElementById('skill-bar'); bar.innerHTML='';
  cls.skills.forEach((sk,i)=>{
    const cd=gameState.combat.cooldowns[i]||0;
    const mp=gameState.player.mp;
    const canUse=cd<=0&&mp>=sk.cost;
    const btn=document.createElement('button');
    btn.className='skill-btn'+(cd>0?' on-cooldown':'');
    btn.disabled=!canUse||!gameState.combat.playerTurn;
    btn.innerHTML=`${sk.icon} ${sk.name}<span class="skill-cd">${sk.dmg} · ${sk.cost}MP${cd>0?' (CD:'+cd+')':''}</span>`;
    btn.addEventListener('click', () => useSkill(i));
    bar.appendChild(btn);
  });
  // Basic attack
  const ba=document.createElement('button'); ba.className='skill-btn';
  ba.disabled=!gameState.combat.playerTurn;
  ba.innerHTML=`⚡ Basic Attack<span class="skill-cd">${Math.floor(gameState.player.atk*0.8)}-${gameState.player.atk} · Free</span>`;
  ba.addEventListener('click', basicAttack);
  bar.appendChild(ba);
}

function useSkill(skillIndex){
  if(!gameState.combat.playerTurn||!gameState.combat.active) return;
  const cls=CLASSES.find(c=>c.id===gameState.player.cls);
  const sk=cls.skills[skillIndex];
  if(gameState.player.mp<sk.cost){ notify('Not enough MP!','danger'); return; }
  gameState.player.mp=Math.max(0,gameState.player.mp-sk.cost);
  gameState.combat.cooldowns[skillIndex]=sk.cd;
  let dmg=0; let msg='';
  if(sk.type==='physical'){
    const [lo,hi]=sk.dmg.split('-').map(Number);
    dmg=calcDamage(lo+(Math.random()*(hi-lo)),gameState.combat.enemy.def,'physical');
    msg=`${sk.icon} ${sk.name}: ${dmg} damage!`;
  } else if(sk.type==='magic'){
    const [lo,hi]=sk.dmg.split('-').map(Number);
    dmg=calcDamage(lo+(Math.random()*(hi-lo)),gameState.combat.enemy.def*0.5,'magic');
    msg=`${sk.icon} ${sk.name}: ${dmg} magic damage!`;
  } else if(sk.type==='buff'){
    if(sk.name==='Shield Wall'){ gameState.player.effects.push({name:'Shield Wall',turns:2,type:'buff'}); dmg=0; msg=`🛡 Shield Wall: Defense +40% for 2 turns!`; }
    else if(sk.name==='Warcry'){ gameState.player.effects.push({name:'Warcry',turns:3,type:'buff'}); dmg=0; msg=`📣 Warcry: ATK +25% for 3 turns!`; }
    else if(sk.name==='Mana Shield'){ gameState.player.effects.push({name:'ManaShield',turns:1,type:'buff'}); dmg=0; msg=`🔵 Mana Shield: Next hit absorbed 30%!`; }
    else if(sk.name==='Smoke Bomb'){ gameState.player.effects.push({name:'Evasion',turns:1,type:'buff'}); dmg=0; msg=`💨 Smoke Bomb: Next attack evaded!`; }
    else if(sk.name==='Death Mark'){ gameState.player.effects.push({name:'Crit',turns:1,type:'buff'}); dmg=0; msg=`💀 Death Mark: Next hit is guaranteed critical!`; }
  } else if(sk.type==='poison'){
    gameState.combat.enemy.effects=gameState.combat.enemy.effects||[];
    gameState.combat.enemy.effects.push({name:'Poison',turns:3,dmg:50,type:'debuff'});
    dmg=calcDamage(40,gameState.combat.enemy.def*0.3,'physical');
    msg=`☠ Poison Blade: ${dmg} + 50 DoT for 3 turns!`;
  } else if(sk.type==='debuff'){
    gameState.combat.enemy.effects=gameState.combat.enemy.effects||[];
    gameState.combat.enemy.effects.push({name:'Stun',turns:2,type:'debuff'});
    gameState.combat.enemy.effects.push({name:'Temporal Burn',turns:3,dmg:35,type:'debuff'});
    dmg=20; msg=`⏳ Temporal Trap: Enemy stunned for 2 turns!`;
  }
  if(dmg>0){ applyDamageToEnemy(dmg); }
  setCombatStatus(msg); addLiveCombatLog(msg,'#e8c060');
  gameState.combat.combo++; showCombo();
  animateAttack('player');
  tickCooldowns();
  if(gameState.combat.enemyHp<=0){ endCombat(true); return; }
  gameState.combat.playerTurn=false;
  buildSkillBar(); updateCombatUI();
  setTimeout(enemyTurn, 1000+Math.random()*800);
}

function basicAttack(){
  if(!gameState.combat.playerTurn||!gameState.combat.active) return;
  let dmg=Math.floor(gameState.player.atk*(0.8+Math.random()*0.4));
  const critEffect=gameState.player.effects.find(e=>e.name==='Crit');
  if(critEffect||Math.random()<0.1){ dmg*=2; setCombatStatus(`⚡ Critical Strike! ${dmg} damage!`); removeEffect('Crit'); }
  else setCombatStatus(`⚡ Basic attack: ${dmg} damage!`);
  dmg=calcDamage(dmg,gameState.combat.enemy.def,'physical');
  applyDamageToEnemy(dmg); animateAttack('player');
  addLiveCombatLog(`⚡ ${dmg} damage`,'#e8c060');
  gameState.combat.combo++;  showCombo(); tickCooldowns();
  if(gameState.combat.enemyHp<=0){ endCombat(true); return; }
  gameState.combat.playerTurn=false;
  buildSkillBar(); updateCombatUI();
  setTimeout(enemyTurn, 1000+Math.random()*800);
}

function enemyTurn(){
  if(!gameState.combat.active||gameState.combat.enemyHp<=0) return;
  const e=gameState.combat.enemy;
  // Process enemy DoT effects
  if(e.effects&&e.effects.length>0){
    e.effects.forEach(ef=>{
      if(ef.dmg){ gameState.combat.enemyHp=Math.max(0,gameState.combat.enemyHp-ef.dmg); addLiveCombatLog(`☠ ${ef.name}: ${ef.dmg} damage!`,'#e87c7c'); }
      ef.turns--;
    });
    e.effects=e.effects.filter(ef=>ef.turns>0);
    if(gameState.combat.enemyHp<=0){ endCombat(true); return; }
  }
  // Check if stunned
  const stunned=e.effects&&e.effects.find(ef=>ef.name==='Stun');
  if(stunned){ setCombatStatus(`⏳ ${e.name} is stunned!`); addLiveCombatLog('⏳ Enemy stunned!','#c0c0c0'); }
  else {
    let dmg=Math.floor(e.atk*(0.8+Math.random()*0.4));
    const evade=gameState.player.effects.find(ef=>ef.name==='Evasion');
    if(evade){ setCombatStatus(`💨 Attack evaded!`); removeEffect('Evasion'); addLiveCombatLog('💨 Evaded!','#c0d0c0'); }
    else {
      const shieldWall=gameState.player.effects.find(ef=>ef.name==='Shield Wall');
      let defMult=shieldWall?1.4:1;
      dmg=Math.max(1,Math.floor(dmg*(1-gameState.player.def*defMult*0.001)));
      const manaShield=gameState.player.effects.find(ef=>ef.name==='ManaShield');
      if(manaShield){ dmg=Math.floor(dmg*0.7); removeEffect('ManaShield'); }
      gameState.player.hp=Math.max(0,gameState.player.hp-dmg);
      setCombatStatus(`${e.icon} ${e.name} attacks for ${dmg} damage!`);
      addLiveCombatLog(`${e.icon} ${dmg} damage!`,'#e87c7c');
      animateHurt('player'); showDamageNumber(dmg,'#e87c7c',false);
    }
  }
  // Tick player effects
  gameState.player.effects.forEach(ef=>{ ef.turns--; });
  gameState.player.effects=gameState.player.effects.filter(ef=>ef.turns>0);
  gameState.combat.round++;
  updateCombatUI();
  if(gameState.player.hp<=0){ endCombat(false); return; }
  gameState.combat.playerTurn=true;
  buildSkillBar();
  // Slight MP regen
  gameState.player.mp=Math.min(gameState.player.mpMax,gameState.player.mp+15);
}

function calcDamage(raw,def,type){
  const atkBoost=gameState.player.effects.find(e=>e.name==='Warcry')?1.25:1;
  return Math.max(1,Math.floor(raw*atkBoost*(1-def*0.0008)));
}

function applyDamageToEnemy(dmg){
  gameState.combat.enemyHp=Math.max(0,gameState.combat.enemyHp-dmg);
  showDamageNumber(dmg,'#e8c060',true); animateHurt('enemy');
}

function removeEffect(name){ gameState.player.effects=gameState.player.effects.filter(e=>e.name!==name); }

function tickCooldowns(){
  for(let k in gameState.combat.cooldowns){ gameState.combat.cooldowns[k]=Math.max(0,gameState.combat.cooldowns[k]-1); }
}

function endCombat(won){
  gameState.combat.active=false;
  if(won){
    const e=gameState.combat.enemy;
    const xpGain=e.xp+Math.floor(Math.random()*20);
    const goldGain=e.gold+Math.floor(Math.random()*30);
    gameState.player.xp+=xpGain; gameState.player.gold+=goldGain;
    gameState.player.kills++; gameState.player.rating+=Math.floor(15+Math.random()*20);
    addLog(`⚔ Slew ${e.name}! +${xpGain}XP, +${goldGain}G`,'kill');
    document.getElementById('victory-screen').classList.remove('hidden');
    document.getElementById('victory-rewards').innerHTML=`
      <div class="reward-item"><div class="reward-value">+${xpGain}</div><div class="reward-label">Experience</div></div>
      <div class="reward-item"><div class="reward-value">+${goldGain}</div><div class="reward-label">Gold</div></div>
      <div class="reward-item"><div class="reward-value">+${Math.floor(15+Math.random()*20)}</div><div class="reward-label">Rating</div></div>
    `;
    checkLevelUp();
  } else {
    gameState.player.deaths++; gameState.player.rating=Math.max(1000,gameState.player.rating-15);
    addLog(`💀 Defeated by ${gameState.combat.enemy.name}.`,'dmg');
    document.getElementById('defeat-screen').classList.remove('hidden');
  }
  updateHUD(); renderRightPanel();
}

function checkLevelUp(){
  while(gameState.player.xp>=gameState.player.xpNext){
    gameState.player.xp-=gameState.player.xpNext;
    gameState.player.level++;
    gameState.player.xpNext=Math.floor(gameState.player.xpNext*1.5);
    const boost=1.12;
    gameState.player.hpMax=Math.floor(gameState.player.hpMax*boost);
    gameState.player.mpMax=Math.floor(gameState.player.mpMax*boost);
    gameState.player.hp=gameState.player.hpMax;
    gameState.player.mp=gameState.player.mpMax;
    gameState.player.atk=Math.floor(gameState.player.atk*1.1);
    gameState.player.def=Math.floor(gameState.player.def*1.1);
    const banner=document.getElementById('level-up-banner');
    document.getElementById('level-up-sub').textContent=`You are now level ${gameState.player.level}`;
    banner.style.display='block';
    setTimeout(()=>banner.style.display='none',3200);
    addLog(`🌟 Level Up! You are now level ${gameState.player.level}!`,'kill');
    notify(`Level Up! Now level ${gameState.player.level}!`,'success');
  }
}

function closeCombat(){
  document.getElementById('combat-frame').classList.add('hidden');
  gameState.player.mp=Math.min(gameState.player.mpMax,gameState.player.mp+Math.floor(gameState.player.mpMax*0.3));
  updateHUD();
}

function respawn(){
  document.getElementById('combat-frame').classList.add('hidden');
  gameState.player.zone='ironhaven'; gameState.player.hp=Math.floor(gameState.player.hpMax*0.5);
  gameState.player.mp=Math.floor(gameState.player.mpMax*0.5);
  addLog(`⟳ Respawned at Ironhaven Shrine.`,'system');
  notify('Respawned at Shrine. -30% Gold penalty.','danger');
  gameState.player.gold=Math.floor(gameState.player.gold*0.7);
  updateHUD(); drawWorld();
}

function fleeCombat(){
  if(Math.random()<0.6){
    gameState.combat.active=false;
    document.getElementById('combat-frame').classList.add('hidden');
    addLog('💨 Escaped from combat!','system'); notify('Fled successfully!','info');
  } else { notify('Failed to flee!','danger'); setCombatStatus('Failed to flee!'); }
}

function startEncounterFromArena(enemyType){
  const pool=ENEMIES.filter(e=>e.type===enemyType||e.type==='elite');
  const enemy=pool[Math.floor(Math.random()*pool.length)];
  startEncounter(enemy);
}