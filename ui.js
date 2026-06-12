// ============================================================
// UI LOGIC
// ============================================================

let worldCanvas, worldCtx, minimapCanvas, minimapCtx;

function buildStars(){
  const sf=document.getElementById('stars');
  for(let i=0;i<120;i++){
    const s=document.createElement('div'); s.className='star';
    s.style.left=Math.random()*100+'%'; s.style.top=Math.random()*100+'%';
    s.style.animationDelay=(Math.random()*3)+'s';
    sf.appendChild(s);
  }
}

function showCharScreen(){
  document.getElementById('title-screen').classList.add('hidden');
  document.getElementById('char-screen').classList.remove('hidden');
  buildClassGrid(); buildFactionGrid();
}

function buildClassGrid(){
  const grid=document.getElementById('class-grid');
  grid.innerHTML=CLASSES.map(c=>`
    <div class="class-card${gameState.player.cls===c.id?' selected':''}" onclick="selectClass('${c.id}')">
      <div class="class-icon">${c.icon}</div>
      <div class="class-name">${c.name}</div>
      <div class="class-role">${c.role}</div>
      <div class="class-desc">${c.desc}</div>
      <div class="class-stats">${c.stats.map(s=>`<span class="stat-pip">${s}</span>`).join('')}</div>
    </div>
  `).join('');
}

function buildFactionGrid(){
  const grid=document.getElementById('faction-grid');
  grid.innerHTML=FACTIONS.map(f=>`
    <div class="faction-card${gameState.player.faction===f.id?' selected':''}" onclick="selectFaction('${f.id}')">
      <div class="faction-name">${f.icon} ${f.name}</div>
      <div class="faction-bonus" style="color:var(--gold-light);margin-top:4px;font-size:0.72rem;">${f.bonus}</div>
      <div class="faction-bonus">${f.desc}</div>
    </div>
  `).join('');
}

function initGameUI(){
  updateHUD();
  initWorldMap();
  renderRightPanel();
  const cls=CLASSES.find(c=>c.id===gameState.player.cls);
  document.getElementById('hud-avatar').textContent=cls.icon;
  document.getElementById('fighter-player').textContent=cls.icon;
  addLog(`⚡ ${gameState.player.name} has entered the Realm.`,'system');
  addLog(`📍 Current zone: Ironhaven City (Safe Zone).`,'system');
  addLog(`⚔ Seek out enemies in PvP zones to earn glory.`,'system');
}

function updateHUD(){
  const p=gameState.player;
  const cls=CLASSES.find(c=>c.id===p.cls);
  document.getElementById('hud-name').textContent=p.name;
  document.getElementById('hud-class').textContent=`${cls?cls.name:'Unknown'} · Lv.${p.level}`;
  document.getElementById('hp-bar').style.width=(p.hp/p.hpMax*100)+'%';
  document.getElementById('hp-val').textContent=`${p.hp}/${p.hpMax}`;
  document.getElementById('mp-bar').style.width=(p.mp/p.mpMax*100)+'%';
  document.getElementById('mp-val').textContent=`${p.mp}/${p.mpMax}`;
  document.getElementById('xp-bar').style.width=(p.xp/p.xpNext*100)+'%';
  document.getElementById('xp-val').textContent=`${p.xp}/${p.xpNext}`;
  document.getElementById('hud-gold').textContent=p.gold.toLocaleString();
  document.getElementById('hud-kills').textContent=p.kills;
  document.getElementById('hud-rating').textContent=p.rating;
  const zone=ZONES.find(z=>z.id===p.zone);
  document.getElementById('hud-zone').textContent=zone?zone.name.split(' ')[0]:'Unknown';
  const pvpFlag=document.getElementById('pvp-flag');
  if(zone&&zone.pvp){ pvpFlag.textContent='⚠ PvP Zone'; pvpFlag.className='pvp-indicator danger'; }
  else{ pvpFlag.textContent='◉ Safe Zone'; pvpFlag.className='pvp-indicator safe'; }
}

function initWorldMap(){
  worldCanvas=document.getElementById('world-canvas');
  minimapCanvas=document.getElementById('minimap-canvas');
  worldCtx=worldCanvas.getContext('2d');
  minimapCtx=minimapCanvas.getContext('2d');
  resizeWorldCanvas();
  window.addEventListener('resize', resizeWorldCanvas);
  worldCanvas.addEventListener('mousemove', onWorldMouseMove);
  worldCanvas.addEventListener('click', onWorldClick);
  worldCanvas.addEventListener('wheel', e=>{ e.preventDefault(); zoomWorld(e.deltaY<0?1.1:0.91); });
  drawWorld();
}

function resizeWorldCanvas(){
  const panel=document.getElementById('world-panel');
  worldCanvas.width=panel.clientWidth;
  worldCanvas.height=panel.clientHeight;
  minimapCanvas.width=160; minimapCanvas.height=120;
  drawWorld();
}

function drawWorld(){
  const w=worldCanvas.width, h=worldCanvas.height;
  const ctx=worldCtx;
  ctx.clearRect(0,0,w,h);
  // Background terrain
  const grd=ctx.createRadialGradient(w*0.5,h*0.5,50,w*0.5,h*0.5,Math.max(w,h)*0.7);
  grd.addColorStop(0,'#111820'); grd.addColorStop(0.5,'#0d1418'); grd.addColorStop(1,'#0a0a0f');
  ctx.fillStyle=grd; ctx.fillRect(0,0,w,h);
  // Grid lines
  ctx.strokeStyle='rgba(255,255,255,0.03)'; ctx.lineWidth=1;
  for(let x=0;x<w;x+=60){ ctx.beginPath(); ctx.moveTo(x,0); ctx.lineTo(x,h); ctx.stroke(); }
  for(let y=0;y<h;y+=60){ ctx.beginPath(); ctx.moveTo(0,y); ctx.lineTo(w,y); ctx.stroke(); }
  // Terrain patches
  const terrains=[
    {x:0.15,y:0.45,rx:0.12,ry:0.08,color:'#1a2a1a'},
    {x:0.65,y:0.3,rx:0.1,ry:0.07,color:'#1a2a1a'},
    {x:0.4,y:0.7,rx:0.15,ry:0.1,color:'#1a2a1a'},
    {x:0.8,y:0.5,rx:0.08,ry:0.06,color:'#1a1a2a'},
    {x:0.25,y:0.2,rx:0.09,ry:0.06,color:'#2a2a1a'},
  ];
  terrains.forEach(t=>{
    ctx.fillStyle=t.color; ctx.beginPath();
    ctx.ellipse(w*t.x*gameState.world.zoom+gameState.world.offsetX,h*t.y*gameState.world.zoom+gameState.world.offsetY,w*t.rx*gameState.world.zoom,h*t.ry*gameState.world.zoom,0,0,Math.PI*2);
    ctx.fill();
  });
  // Roads
  ctx.strokeStyle='rgba(201,168,76,0.08)'; ctx.lineWidth=3; ctx.setLineDash([8,12]);
  const roads=[[0.35,0.45,0.12,0.3],[0.35,0.45,0.6,0.25],[0.35,0.45,0.25,0.65],[0.35,0.45,0.55,0.72],[0.35,0.45,0.42,0.2],[0.35,0.45,0.75,0.6]];
  roads.forEach(([x1,y1,x2,y2])=>{
    ctx.beginPath();
    ctx.moveTo(w*x1*gameState.world.zoom+gameState.world.offsetX,h*y1*gameState.world.zoom+gameState.world.offsetY);
    ctx.lineTo(w*x2*gameState.world.zoom+gameState.world.offsetX,h*y2*gameState.world.zoom+gameState.world.offsetY);
    ctx.stroke();
  });
  ctx.setLineDash([]);
  // Zones
  ZONES.forEach(z=>{
    const zx=w*z.x*gameState.world.zoom+gameState.world.offsetX;
    const zy=h*z.y*gameState.world.zoom+gameState.world.offsetY;
    const sz=z.size*gameState.world.zoom;
    const isHovered=gameState.world.hoveredZone===z.id;
    const isSelected=gameState.world.selectedZone===z.id;
    const isCurrent=gameState.player.zone===z.id;
    ctx.fillStyle=z.color+(isHovered?'99':'55');
    ctx.strokeStyle=isCurrent?'#c9a84c':isHovered?'rgba(255,255,255,0.4)':'rgba(255,255,255,0.15)';
    ctx.lineWidth=isCurrent?2.5:isHovered?1.5:0.5;
    ctx.beginPath(); ctx.arc(zx,zy,sz*0.5,0,Math.PI*2);
    ctx.fill(); ctx.stroke();
    if(z.pvp){
      ctx.strokeStyle='rgba(192,57,43,0.4)'; ctx.lineWidth=1; ctx.setLineDash([4,4]);
      ctx.beginPath(); ctx.arc(zx,zy,sz*0.5+8,0,Math.PI*2); ctx.stroke(); ctx.setLineDash([]);
    }
    ctx.font=(14*gameState.world.zoom)+'px Segoe UI'; ctx.textAlign='center'; ctx.fillStyle=isCurrent?'#f0d080':'#d0c8b8';
    ctx.fillText(z.icon+' '+z.name,zx,zy+sz*0.5+18*gameState.world.zoom);
    if(z.pvp){ ctx.font=(10*gameState.world.zoom)+'px Segoe UI'; ctx.fillStyle='#e74c3c'; ctx.fillText('⚔ PvP',zx,zy+sz*0.5+30*gameState.world.zoom); }
    if(isCurrent){ ctx.font=(11*gameState.world.zoom)+'px Segoe UI'; ctx.fillStyle='#c9a84c'; ctx.fillText('▼ You are here',zx,zy-sz*0.5-8); }
  });
  drawMinimap();
}

function drawMinimap(){
  const mw=160,mh=120;
  minimapCtx.fillStyle='#0a0a14'; minimapCtx.fillRect(0,0,mw,mh);
  minimapCtx.strokeStyle='rgba(255,255,255,0.05)'; minimapCtx.lineWidth=0.5;
  ZONES.forEach(z=>{
    const zx=mw*z.x, zy=mh*z.y, r=Math.max(4,z.size*0.07);
    minimapCtx.fillStyle=z.color+'aa';
    minimapCtx.beginPath(); minimapCtx.arc(zx,zy,r,0,Math.PI*2); minimapCtx.fill();
    if(gameState.player.zone===z.id){
      minimapCtx.fillStyle='#c9a84c'; minimapCtx.beginPath();
      minimapCtx.arc(zx,zy,3,0,Math.PI*2); minimapCtx.fill();
    }
  });
}

function onWorldMouseMove(e){
  const rect=worldCanvas.getBoundingClientRect();
  const mx=e.clientX-rect.left, my=e.clientY-rect.top;
  const w=worldCanvas.width, h=worldCanvas.height;
  let hit=null;
  ZONES.forEach(z=>{
    const zx=w*z.x*gameState.world.zoom+gameState.world.offsetX;
    const zy=h*z.y*gameState.world.zoom+gameState.world.offsetY;
    const sz=z.size*0.5*gameState.world.zoom;
    const dist=Math.hypot(mx-zx,my-zy);
    if(dist<sz) hit=z;
  });
  const prev=gameState.world.hoveredZone;
  gameState.world.hoveredZone=hit?hit.id:null;
  if(gameState.world.hoveredZone!==prev) drawWorld();
  const tt=document.getElementById('zone-tooltip');
  const tb=document.getElementById('travel-btn');
  if(hit){
    tt.style.display='block'; tt.style.left=(mx+16)+'px'; tt.style.top=(my-8)+'px';
    document.getElementById('tt-name').textContent=hit.icon+' '+hit.name;
    document.getElementById('tt-level').textContent='Level Range: '+hit.level;
    document.getElementById('tt-type').textContent='Type: '+hit.type;
    document.getElementById('tt-desc').textContent=hit.desc;
    document.getElementById('tt-players').textContent=hit.pvp?'⚔ PvP Enabled — Full Loot':'✓ Safe Zone';
    tb.style.display='block'; tb.style.left=(mx+16)+'px'; tb.style.top=(my+80)+'px';
    tb.dataset.zone=hit.id;
    tb.textContent=gameState.player.zone===hit.id?'✓ Current Location':'⚡ Travel Here';
  } else { tt.style.display='none'; tb.style.display='none'; }
}

function onWorldClick(e){
  if(gameState.world.hoveredZone){ travelToZone(gameState.world.hoveredZone); }
}

function renderRightPanel(){
  const c=document.getElementById('right-content');
  if(gameState.rightTab==='log') c.innerHTML=buildCombatLogHTML();
  else if(gameState.rightTab==='lb') c.innerHTML=buildLeaderboardHTML();
}

function buildCombatLogHTML(){
  return `<div class="combat-log-header"><span>Combat Log</span><span style="color:var(--text-dim)">${gameState.combatLog.length} entries</span></div>
  <div class="combat-log">${gameState.combatLog.slice().reverse().slice(0,40).map(e=>`<div class="log-entry ${e.type}">${e.msg}</div>`).join('')}</div>`;
}

function buildLeaderboardHTML(){
  return `<div class="leaderboard-title">PvP Rankings — Eternal Server</div>
  <div class="lb-tabs">
    <button class="lb-tab active" onclick="">PvP</button>
    <button class="lb-tab" onclick="">Kills</button>
    <button class="lb-tab" onclick="">Rating</button>
  </div>
  ${LEADERBOARD_PVP.map((p,i)=>{
    const rankClass=i===0?'gold':i===1?'silver':i===2?'bronze':'';
    return `<div class="lb-entry">
      <div class="lb-rank ${rankClass}">${i===0?'👑':i===1?'🥈':i===2?'🥉':p.rank}</div>
      <div class="lb-name">${p.icon} ${p.name}</div>
      <div class="lb-class-badge" style="font-size:0.6rem;color:var(--text-muted)">${p.cls}</div>
      <div class="lb-score">${p.score.toLocaleString()}</div>
    </div>`;
  }).join('')}
  <div style="margin-top:0.75rem;padding:0.5rem;background:var(--bg-card);border:1px solid var(--border-gold);border-radius:4px;text-align:center;">
    <div style="font-size:0.7rem;color:var(--text-muted)">Your Ranking</div>
    <div style="font-size:1rem;color:var(--gold);font-weight:600">#${Math.floor(200+Math.random()*800)}</div>
    <div style="font-size:0.7rem;color:var(--text-muted)">${gameState.player.rating} Rating · ${gameState.player.kills} Kills</div>
  </div>`;
}

function switchTab(tab){
  gameState.navTab=tab;
  ['world','pvp','social','market','char'].forEach(t=>{
    document.getElementById('tab-'+t).classList.remove('active');
  });
  document.getElementById('tab-'+tab).classList.add('active');
  const wp=document.getElementById('world-panel');
  const rp=document.querySelector('.right-panel');
  if(tab==='world'){ wp.style.display=''; rp.style.display=''; renderRightPanel(); }
  else { wp.style.display='none'; rp.style.display='none'; showFullPanel(tab); }
}

function showFullPanel(tab){
  const ma=document.querySelector('.main-area');
  let existing=document.getElementById('full-panel');
  if(existing) existing.remove();
  const div=document.createElement('div'); div.id='full-panel';
  div.style.cssText='flex:1;overflow-y:auto;background:var(--bg-deep);padding:1.5rem;';
  if(tab==='pvp') div.innerHTML=buildPvPPanel();
  else if(tab==='social') div.innerHTML=buildSocialPanel();
  else if(tab==='market') div.innerHTML=buildMarketPanel();
  else if(tab==='char') div.innerHTML=buildCharPanel();
  ma.appendChild(div);
  div.querySelectorAll('[data-queue]').forEach(btn=>{
    btn.addEventListener('click',()=>toggleQueue(btn.dataset.queue));
  });
}

function buildPvPPanel(){
  const fw=gameState.factionWar;
  const total=fw.ironveil+fw.sunken+fw.voidborn;
  return `
  <div style="max-width:900px;margin:0 auto;">
    <div style="font-size:1.5rem;color:var(--gold);font-weight:600;margin-bottom:0.25rem;">⚔ PvP Arenas</div>
    <div style="color:var(--text-muted);font-size:0.8rem;margin-bottom:2rem;">Real-time PvP matchmaking · Ranked & Unranked modes · Faction Wars</div>
    
    <div class="pvp-section">
      <div class="pvp-section-title">Ranked Arenas</div>
      <div style="display:grid;grid-template-columns:1fr 1fr;gap:1rem;">
        ${[
          {id:'Duel-1v1',title:'1v1 Duel Arena',mode:'Ranked · Single Combat',reward:'+20 Rating · Gold',type:'red'},
          {id:'Team-3v3',title:'3v3 Team Skirmish',mode:'Ranked · Team Tactics',reward:'+15 Rating/player · Gear',type:'purple'},
          {id:'BG-10v10',title:'10v10 Battleground',mode:'Ranked · Objective Hold',reward:'+Faction Points · Epics',type:'blue'},
          {id:'FFA-8',title:'8-Player FFA Mayhem',mode:'Unranked · Last Man Standing',reward:'+Chaos Tokens · Fun',type:''},
        ].map(a=>`
        <div class="arena-card ${a.type?a.type+'-accent':''}">
          <div class="arena-title">${a.title}</div>
          <div class="arena-meta"><span>Mode: ${a.mode}</span></div>
          <div class="arena-reward">Reward: ${a.reward}</div>
          <button class="queue-btn ${gameState.pvpQueue===a.id?'queued':''}" data-queue="${a.id}">
            ${gameState.pvpQueue===a.id?'⏳ In Queue... (click to leave)':'+ Join Queue'}
          </button>
        </div>`).join('')}
      </div>
    </div>

    <div class="pvp-section">
      <div class="pvp-section-title">Faction War</div>
      <div class="faction-war-bar">
        <div class="faction-war-title">⚑ Current Dominion — Server-Wide Territory Control</div>
        <div class="faction-bar">
          <div class="faction-bar-fill ironveil" style="width:${fw.ironveil/total*100}%"></div>
          <div class="faction-bar-fill sunken" style="width:${fw.sunken/total*100}%"></div>
          <div class="faction-bar-fill voidborn" style="width:${fw.voidborn/total*100}%"></div>
        </div>
        <div class="faction-labels">
          <span style="color:#6a8fa8">⚙ Ironveil ${fw.ironveil}%</span>
          <span style="color:#8a6a2a">🌊 Sunken ${fw.sunken}%</span>
          <span style="color:#6a2a8a">🌀 Voidborn ${fw.voidborn}%</span>
        </div>
        <button onclick="factionAttack()" style="margin-top:0.75rem;width:100%;background:rgba(201,168,76,0.1);border:1px solid var(--gold);color:var(--gold);padding:0.5rem;font-size:0.75rem;letter-spacing:1px;text-transform:uppercase;cursor:pointer;border-radius:3px;font-family:inherit;">⚔ Contribute to Faction War</button>
      </div>
    </div>

    <div class="pvp-section">
      <div class="pvp-section-title">Bounty Board</div>
      ${gameState.bounties.map(b=>`
      <div class="bounty-entry">
        <div class="bounty-avatar">${b.icon}</div>
        <div class="bounty-name"><span>${b.name}</span><span>${b.cls}</span></div>
        <div style="display:flex;flex-direction:column;align-items:flex-end;gap:4px;">
          <div class="bounty-gold">${b.reward.toLocaleString()} G</div>
          <button onclick="notify('Tracking ${b.name}...','danger')" style="background:rgba(192,57,43,0.1);border:1px solid var(--red);color:var(--red-light);padding:2px 8px;font-size:0.6rem;cursor:pointer;border-radius:3px;font-family:inherit;letter-spacing:1px;text-transform:uppercase;">Hunt</button>
        </div>
      </div>`).join('')}
    </div>

    <div class="pvp-section">
      <div class="pvp-section-title">World PvP Objectives</div>
      <div style="display:grid;grid-template-columns:repeat(3,1fr);gap:0.75rem;">
        ${[
          {name:'Crimson Fortress',status:'Contested',holder:'None',bonus:'20% XP boost'},
          {name:'Void Nexus Tower',status:'Ironveil',holder:'Ironveil',bonus:'Teleport access'},
          {name:'Ancient Shrine',status:'Sunken',holder:'Sunken Court',bonus:'+15% gold drops'},
        ].map(o=>`
        <div style="background:var(--bg-card);border:1px solid var(--border);padding:0.75rem;border-radius:4px;">
          <div style="font-size:0.82rem;color:var(--text);font-weight:600;">${o.name}</div>
          <div style="font-size:0.65rem;color:${o.status==='Contested'?'var(--orange-light)':'var(--green-light)'};margin-top:3px;">${o.status}</div>
          <div style="font-size:0.65rem;color:var(--text-muted);margin-top:2px;">Holder: ${o.holder}</div>
          <div style="font-size:0.65rem;color:var(--gold-light);margin-top:2px;">Bonus: ${o.bonus}</div>
          <button onclick="notify('Traveling to ${o.name}...','info')" style="width:100%;margin-top:0.5rem;background:transparent;border:1px solid var(--border-gold);color:var(--gold);padding:3px;font-size:0.65rem;cursor:pointer;border-radius:3px;font-family:inherit;letter-spacing:1px;text-transform:uppercase;">Capture</button>
        </div>`).join('')}
      </div>
    </div>
  </div>`;
}

function buildSocialPanel(){
  return `
  <div style="max-width:900px;margin:0 auto;">
    <div style="font-size:1.5rem;color:var(--gold);font-weight:600;margin-bottom:0.25rem;">⚑ Guild & Social</div>
    <div style="color:var(--text-muted);font-size:0.8rem;margin-bottom:2rem;">Guilds, friends, global chat, and cooperation</div>
    <div style="display:grid;grid-template-columns:1.5fr 1fr;gap:1.5rem;">
      <div>
        <div class="social-title">Your Guild</div>
        <div class="guild-card-main">
          <div style="display:flex;justify-content:space-between;align-items:flex-start;">
            <div>
              <div class="guild-name-display">⚔ Iron Vanguard</div>
              <div class="guild-meta">Guild Level 12 · 47 Members · Top 15 Server</div>
              <div style="margin-top:0.5rem;display:flex;gap:0.5rem;flex-wrap:wrap;">
                <span style="font-size:0.65rem;background:rgba(201,168,76,0.12);border:1px solid var(--border-gold);color:var(--gold);padding:2px 6px;border-radius:2px;">Rank: Officer</span>
                <span style="font-size:0.65rem;background:rgba(26,122,58,0.12);border:1px solid var(--green);color:var(--green-light);padding:2px 6px;border-radius:2px;">War: Active</span>
              </div>
            </div>
            <div style="text-align:right;">
              <div style="font-size:1.5rem">⚔</div>
              <div style="font-size:0.65rem;color:var(--text-muted)">Guild Emblem</div>
            </div>
          </div>
          <div style="margin-top:0.75rem;font-size:0.7rem;color:var(--text-muted)">Weekly Guild XP Progress</div>
          <div class="guild-xp-bar"><div class="guild-xp-fill" style="width:67%"></div></div>
          <div style="font-size:0.65rem;color:var(--text-muted);margin-top:3px;">67,000 / 100,000 XP to next tier</div>
        </div>
        
        <div class="social-title" style="margin-top:1rem;">Guild Log</div>
        ${[
          {icon:'⚔',msg:'TitanBane defeated 12 players in Void Rift',time:'2m ago',color:'var(--gold)'},
          {icon:'🌟',msg:'Guild reached Level 12! New perks unlocked',time:'1h ago',color:'var(--green-light)'},
          {icon:'💀',msg:'PhaseWalker was slain by VoidReaper',time:'2h ago',color:'var(--red-light)'},
          {icon:'🏆',msg:'Iron Vanguard won the weekly siege!',time:'1d ago',color:'var(--gold)'},
        ].map(l=>`
        <div style="display:flex;gap:0.6rem;padding:0.4rem;background:var(--bg-card);border:1px solid var(--border);margin-bottom:3px;border-radius:3px;">
          <span style="font-size:1rem;">${l.icon}</span>
          <div style="flex:1;">
            <div style="font-size:0.75rem;color:${l.color}">${l.msg}</div>
            <div style="font-size:0.6rem;color:var(--text-dim)">${l.time}</div>
          </div>
        </div>`).join('')}
      </div>
      <div>
        <div class="social-title">Guild Members Online</div>
        <div class="online-list">
          ${GUILD_MEMBERS.map(m=>`
          <div class="online-entry">
            <div class="online-dot ${m.status}"></div>
            <div class="player-avatar" style="width:24px;height:24px;font-size:0.75rem;border-width:1px;">${m.cls}</div>
            <div style="flex:1;">
              <div class="online-player-name">${m.name}</div>
              <div class="online-player-zone">${m.zone}</div>
            </div>
            <button onclick="notify('Inviting ${m.name} to party...','info')" style="background:transparent;border:1px solid var(--border);color:var(--text-dim);padding:1px 6px;font-size:0.6rem;cursor:pointer;border-radius:2px;font-family:inherit;">+Party</button>
          </div>`).join('')}
        </div>
        <div class="social-title" style="margin-top:1rem;">Global Chat</div>
        <div style="background:var(--bg-card);border:1px solid var(--border);border-radius:4px;padding:0.5rem;height:180px;overflow-y:auto;display:flex;flex-direction:column;gap:2px;">
          ${[
            {name:'VoidReaper',msg:'Anyone want to do Void Rift raid tonight?',faction:'🌀'},
            {name:'IronCrown',msg:'LFG for 3v3 ranked, need healer',faction:'⚙'},
            {name:'ArcaneStorm',msg:'WTS Dragon Scale, 11k OBO',faction:'🌀'},
            {name:'TitanBane',msg:'Sunken Court is taking the fortress!',faction:'⚙'},
            {name:'BladeRunner',msg:'GG that last war front',faction:'🌊'},
            {name:'MysticFlame',msg:'Echoes MMO best game 2025',faction:'🌊'},
          ].map(c=>`<div style="font-size:0.72rem;"><span style="color:var(--gold-light)">${c.faction} ${c.name}:</span> <span style="color:var(--text-muted)">${c.msg}</span></div>`).join('')}
        </div>
        <div style="display:flex;gap:0.4rem;margin-top:0.4rem;">
          <input id="chat-input" placeholder="Say something..." style="flex:1;background:var(--bg-card);border:1px solid var(--border);color:var(--text);padding:4px 8px;font-size:0.75rem;border-radius:3px;font-family:inherit;" />
          <button onclick="sendChat()" style="background:rgba(201,168,76,0.1);border:1px solid var(--border-gold);color:var(--gold);padding:4px 8px;font-size:0.7rem;cursor:pointer;border-radius:3px;font-family:inherit;">Send</button>
        </div>
      </div>
    </div>
  </div>`;
}

function sendChat(){
  const inp=document.getElementById('chat-input');
  if(!inp||!inp.value.trim()) return;
  notify(`Said: "${inp.value}"`, 'info'); inp.value='';
}

function buildMarketPanel(){
  return `
  <div style="max-width:900px;margin:0 auto;">
    <div style="font-size:1.5rem;color:var(--gold);font-weight:600;margin-bottom:0.25rem;">⚖ Auction House</div>
    <div style="color:var(--text-muted);font-size:0.8rem;margin-bottom:1.5rem;">Player-driven economy · Real-time bids · 5% listing fee</div>
    <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:1rem;">
      <div class="market-filter">
        ${['All','Weapon','Armor','Consumable','Material','Epic+'].map((f,i)=>`<button class="filter-chip${i===0?' active':''}" onclick="filterMarket('${f}',this)">${f}</button>`).join('')}
      </div>
      <div style="font-size:0.75rem;color:var(--gold);">💰 Your Gold: ${gameState.player.gold.toLocaleString()}</div>
    </div>
    <div id="market-list">
      ${MARKET_ITEMS.map(item=>`
      <div class="market-item" id="mi-${item.name.replace(/\s/g,'-')}">
        <div class="item-rarity ${item.rarity}"></div>
        <div class="item-icon">${item.icon}</div>
        <div class="item-info">
          <div class="item-name" style="color:${item.rarity==='legendary'?'var(--gold)':item.rarity==='epic'?'#9b59b6':item.rarity==='rare'?'#3498db':item.rarity==='uncommon'?'var(--green-light)':'var(--text)'}">${item.name}</div>
          <div class="item-stats-small">${item.type} · ${item.stats}</div>
        </div>
        <div class="item-price">
          <span class="item-price-val">💰 ${item.price.toLocaleString()}</span>
          <span class="item-price-qty">Qty: ${item.qty}</span>
          <button class="buy-item-btn" onclick="buyItem('${item.name}',${item.price})">Buy</button>
        </div>
      </div>`).join('')}
    </div>
  </div>`;
}

function filterMarket(filter, btn){
  document.querySelectorAll('.filter-chip').forEach(b=>b.classList.remove('active'));
  btn.classList.add('active');
  document.querySelectorAll('.market-item').forEach(el=>el.style.display='');
  if(filter==='All') return;
  MARKET_ITEMS.forEach(item=>{
    const el=document.getElementById('mi-'+item.name.replace(/\s/g,'-'));
    if(!el) return;
    const match=filter==='Epic+'?(item.rarity==='epic'||item.rarity==='legendary'):item.type===filter;
    el.style.display=match?'':'none';
  });
}

function buildCharPanel(){
  const p=gameState.player;
  const cls=CLASSES.find(c=>c.id===p.cls);
  const fac=FACTIONS.find(f=>f.id===p.faction);
  return `
  <div style="max-width:900px;margin:0 auto;">
    <div style="font-size:1.5rem;color:var(--gold);font-weight:600;margin-bottom:0.25rem;">◈ Character Sheet</div>
    <div style="color:var(--text-muted);font-size:0.8rem;margin-bottom:1.5rem;">Stats, abilities, and progression</div>
    <div style="display:grid;grid-template-columns:1fr 2fr;gap:1.5rem;">
      <div>
        <div class="char-detail-panel clearfix">
          <div class="char-portrait">${cls?cls.icon:'⚔'}</div>
          <div style="overflow:hidden">
            <div class="char-detail-name">${p.name}</div>
            <div class="char-detail-class">${cls?cls.name:'Unknown'} · ${cls?cls.role:''}</div>
            <div class="char-detail-level">Level ${p.level} Champion</div>
            <div class="char-detail-faction">${fac?fac.icon+' '+fac.name:''}</div>
          </div>
        </div>
        <div style="height:1px;background:var(--border);margin:0.75rem 0;"></div>
        <div class="stat-grid">
          <div class="stat-cell"><div class="stat-cell-label">Attack</div><div class="stat-cell-val">${p.atk}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Defense</div><div class="stat-cell-val">${p.def}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Speed</div><div class="stat-cell-val">${p.spd}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Max HP</div><div class="stat-cell-val">${p.hpMax}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Max MP</div><div class="stat-cell-val">${p.mpMax}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Rating</div><div class="stat-cell-val" style="color:var(--gold)">${p.rating}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Kills</div><div class="stat-cell-val" style="color:var(--red-light)">${p.kills}</div></div>
          <div class="stat-cell"><div class="stat-cell-label">Deaths</div><div class="stat-cell-val">${p.deaths}</div></div>
        </div>
        <div style="height:1px;background:var(--border);margin:0.75rem 0;"></div>
        <div style="font-size:0.65rem;color:var(--text-dim);text-transform:uppercase;letter-spacing:2px;margin-bottom:0.5rem;">Achievements</div>
        ${[
          {icon:'⚔',name:'First Blood',desc:'Win your first PvP fight',done:p.kills>0},
          {icon:'💀',name:'Serial Killer',desc:'Defeat 10 enemies',done:p.kills>=10},
          {icon:'🌟',name:'Rising Star',desc:'Reach level 5',done:p.level>=5},
          {icon:'💰',name:'Gold Hoarder',desc:'Accumulate 5,000 gold',done:p.gold>=5000},
        ].map(a=>`
        <div style="display:flex;align-items:center;gap:0.5rem;padding:0.35rem 0.5rem;background:var(--bg-card);border:1px solid ${a.done?'var(--border-gold)':'var(--border)'};margin-bottom:3px;border-radius:3px;opacity:${a.done?1:0.5}">
          <span>${a.icon}</span>
          <div style="flex:1;"><div style="font-size:0.75rem;color:${a.done?'var(--gold-light)':'var(--text)'}">${a.name}</div><div style="font-size:0.62rem;color:var(--text-muted)">${a.desc}</div></div>
          <span style="font-size:0.7rem">${a.done?'✓':''}</span>
        </div>`).join('')}
      </div>
      <div>
        <div class="skills-list">
          <h4>Class Abilities — ${cls?cls.name:''}</h4>
          ${(cls?cls.skills:[]).map(sk=>`
          <div class="skill-entry">
            <div class="skill-entry-icon">${sk.icon}</div>
            <div style="flex:1;">
              <div class="skill-entry-name">${sk.name}</div>
              <div class="skill-entry-desc">${sk.desc}</div>
            </div>
            <div class="skill-entry-dmg">
              <div style="color:var(--gold-light)">${sk.dmg}</div>
              <div style="font-size:0.6rem;color:var(--text-muted)">${sk.cost}MP · ${sk.cd}t CD</div>
            </div>
          </div>`).join('')}
        </div>
        <div style="height:1px;background:var(--border);margin:1rem 0;"></div>
        <div style="font-size:0.65rem;color:var(--text-dim);text-transform:uppercase;letter-spacing:2px;margin-bottom:0.5rem;">Equipment Slots</div>
        <div style="display:grid;grid-template-columns:1fr 1fr 1fr;gap:0.5rem;">
          ${['Helm','Chest','Legs','Weapon','Offhand','Boots','Gloves','Ring','Amulet'].map(slot=>`
          <div style="background:var(--bg-card);border:1px solid var(--border);padding:0.5rem;border-radius:3px;text-align:center;min-height:60px;display:flex;flex-direction:column;align-items:center;justify-content:center;cursor:pointer;" onclick="notify('${slot} slot — No item equipped.','info')">
            <div style="font-size:0.6rem;color:var(--text-dim);text-transform:uppercase;letter-spacing:1px;">${slot}</div>
            <div style="font-size:0.65rem;color:var(--text-muted);margin-top:2px;">Empty</div>
          </div>`).join('')}
        </div>
        <div style="height:1px;background:var(--border);margin:1rem 0;"></div>
        <div style="font-size:0.65rem;color:var(--text-dim);text-transform:uppercase;letter-spacing:2px;margin-bottom:0.5rem;">Faction Bonus: ${fac?fac.name:''}</div>
        <div style="background:var(--bg-card);border:1px solid var(--border-gold);padding:0.75rem;border-radius:4px;">
          <div style="font-size:0.82rem;color:var(--gold-light)">${fac?fac.bonus:''}</div>
          <div style="font-size:0.7rem;color:var(--text-muted);margin-top:0.3rem">${fac?fac.desc:''}</div>
        </div>
      </div>
    </div>
  </div>`;
}

function notify(msg,type='info'){
  const n=document.getElementById('notifications');
  const el=document.createElement('div'); el.className=`notif ${type}`;
  el.textContent=msg; n.appendChild(el);
  setTimeout(()=>el.remove(),4000);
}

function addLiveCombatLog(msg,color){
  const ll=document.getElementById('live-log');
  const el=document.createElement('div'); el.className='live-log';
  el.style.color=color; el.textContent=msg; ll.appendChild(el);
  setTimeout(()=>el.remove(),3500);
}

function setCombatStatus(msg){ document.getElementById('combat-status').textContent=msg; }

function showDamageNumber(dmg,color,isEnemy){
  const arena=document.getElementById('combat-arena');
  const el=document.createElement('div'); el.className='damage-number';
  el.style.color=color; el.style.left=(isEnemy?'55%':'28%'); el.style.top='40%';
  el.textContent='-'+dmg; arena.appendChild(el);
  setTimeout(()=>el.remove(),1200);
}

function showCombo(){
  const c=gameState.combat.combo;
  const cd=document.getElementById('combo-display');
  const ct=document.getElementById('combo-text');
  if(c>=3){ cd.style.display='block'; ct.textContent=c+'x COMBO!'; setTimeout(()=>cd.style.display='none',900); }
}

function animateAttack(who){
  const el=document.getElementById(who==='player'?'fighter-player':'fighter-enemy');
  el.classList.remove('attack-anim'); void el.offsetWidth; el.classList.add('attack-anim');
  setTimeout(()=>el.classList.remove('attack-anim'),400);
}
function animateHurt(who){
  const el=document.getElementById(who==='player'?'fighter-player':'fighter-enemy');
  el.classList.remove('hurt-anim'); void el.offsetWidth; el.classList.add('hurt-anim');
  setTimeout(()=>el.classList.remove('hurt-anim'),400);
}

function updateCombatUI(){
  const p=gameState.player, e=gameState.combat;
  document.getElementById('c-player-hp').style.width=(p.hp/p.hpMax*100)+'%';
  document.getElementById('c-player-hp-text').textContent=p.hp+' / '+p.hpMax+' HP';
  document.getElementById('c-player-mp').style.width=(p.mp/p.mpMax*100)+'%';
  document.getElementById('c-player-mp-text').textContent=p.mp+' / '+p.mpMax+' MP';
  document.getElementById('c-enemy-hp').style.width=(e.enemyHp/e.enemyHpMax*100)+'%';
  document.getElementById('c-enemy-hp-text').textContent=e.enemyHp+' / '+e.enemyHpMax+' HP';
  document.getElementById('round-display').textContent='Round '+e.round;
  // Effects
  const pEffects=document.getElementById('player-effects');
  pEffects.innerHTML=p.effects.map(ef=>`<span class="effect-tag buff">${ef.name}(${ef.turns})</span>`).join('');
  const eEffects=document.getElementById('enemy-effects');
  eEffects.innerHTML=(e.enemy.effects||[]).map(ef=>`<span class="effect-tag debuff">${ef.name}(${ef.turns})</span>`).join('');
  updateHUD();
}