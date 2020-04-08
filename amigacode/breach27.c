/* breach27.c - Enemy Attack */

#include   "breach2.h"


/* External variables */ 
extern long cdel;
extern FILE *ser;
extern Byte maragun[],gungun1[],gungun2[],tankgun[],bipedgun1[],bipedgun2[];
extern Byte robotgun1[],robotgun2[],aliengun[];
extern struct QstStru *Quest;
extern Byte obs[5][50][50];
extern Byte mar[5][50][50];
extern int MapWindow;
extern Point MapOrigin;
extern int Level;
extern int CurMove;
extern int CurEnemy,curpath,curx,cury,destx,desty,targx,targy;
extern int tnum,trng,tdir;
extern Boolean outrng;
extern int quills2[3][4],quills3[3][6],quills4[3][14];



ClearMstats()

/* clear the enemy window */

{
   SetColor(15);
   RectangleFill(222,61,318,143);
   }
   
   
   
TurnToFace()

/* Change facing of enemy toward target */

{
   if(targx < curx) {
      if(targy < cury) Quest->opponent[CurEnemy].facing = 7;
      else if(targy > cury) Quest->opponent[CurEnemy].facing = 5;
      else Quest->opponent[CurEnemy].facing = 6;
	  }
   else if(targx > curx) {
      if(targy < cury) Quest->opponent[CurEnemy].facing = 1;
      else if(targy > cury) Quest->opponent[CurEnemy].facing = 3;
      else Quest->opponent[CurEnemy].facing = 2;
	  }
   else  {
      if(targy < cury) Quest->opponent[CurEnemy].facing = 0;
      else Quest->opponent[CurEnemy].facing = 4;
	  }
   }
   
   

Zap()

/* Do the damage */

{
   int x,y;
   
   /* Damage special effect */
   x = targx - MapOrigin.x;
   y = targy - MapOrigin.y;
   BlitCreature(95,x,y,0,notSrcBic);
   BlitCreature(94,x,y,0,srcOr);
   Delay(3L);
   BlitCreature(97,x,y,0,notSrcBic);
   BlitCreature(96,x,y,0,srcOr);
   Delay(3L);
   Damage(Quest->opponent[CurEnemy].gun,Level,targx,targy);
   DrawMapWindow();
   if(MapWindow != NULL)
    if(Quest->marine[tnum].condition > 0) PrintLineXY("hit.",235,85);
    else PrintLineXY("killed.",235,85);
   Delay(cdel);
   }
  
   
   
EnemyRocketFire()

{
   int x,y,i,j,x1,y1,x2,y2,p,s1,f,dx,dy,tx,ty,m;
   Point p2,p1;
   char astr[30];

   SoundEff(10);
   TurnToFace();
   DrawMapWindow();
   ClearMstats();
   PrintLineXY("Rocket:",235,65);
   strptoc(astr,Quest->marine[tnum].name);
   astr[12] = 0;
   PrintLineXY(astr,235,75);
   Delay(10L);
   f = Quest->opponent[CurEnemy].facing;
   x = curx;
   y = cury;
   /* Set one of the coordinates to the map edge */
   switch(f) {
      case 0: x1 = x;
              y1 = 0;
			  dx = 0;
			  dy = -1;
			  m = 383;
              break;
      case 1: if(x+y > 49) {
	             x1 = 49;
				 y1 = y - (49-x);
				 }
			  else if(x+y < 49) {
			     y1 = 0;
				 x1 = x + y;
				 }
			  else {
			     y1 = 0;
				 x1 = 49;
				 }
			  dx = 1;
			  dy = -1;
			  m = 385;
              break;
      case 2: y1 = MapOrigin.y+4;
              x1 = 49;
			  dx = 1;
			  dy = 0;
			  m = 387;
              break;
      case 3: if(x > y) {
	             x1 = 49;
				 y1 = y + (49-x);
				 }
			  else if(x < y) {
			     y1 = 49;
				 x1 = x + (49-y);
				 }
			  else {
			     y1 = 49;
				 x1 = 49;
				 }
			  dx = 1;
			  dy = 1;
			  m = 389;
              break;
      case 4: x1 = MapOrigin.x+4;
              y1 = 49;
			  dx = 0;
			  dy = 1;
			  m = 397;
              break;
      case 5: if(x+y > 49) {
	             y1 = 49;
				 x1 = x - (49-y);
				 }
			  else if(x+y < 49) {
			     x1 = 0;
				 y1 = x + y;
				 }
			  else {
			     y1 = 49;
				 x1 = 0;
				 }
			  dx = -1;
			  dy = 1;
			  m = 399;
			  break;
      case 6: y1 = MapOrigin.y+4;
              x1 = 0;
			  dx = -1;
			  dy = 0;
			  m = 401;
              break;
      case 7: if(x > y) {
	             x1 = x - y;
				 y1 = 0;
				 }
			  else if(x < y) {
			     y1 = y - x;
				 x1 = 0;
				 }
			  else {
			     y1 = 0;
				 x1 = 0;
				 }
			  dx = -1;
			  dy = -1;
			  m = 409;
              break;
		 }
   /* Now, find the target square */
   FindTarget(x1,y1,50,&x,&y);
   SetPt(&p2,(x-MapOrigin.x)*24+12,(y-MapOrigin.y)*18+25);
   x1 = 108;
   y1 = 97;
   x2 = p2.x;
   y2 = p2.y;
   if(m == 389) i = 7;
   else i = 1;
   SavePartial();
   SoundEff(18);
   do {
      tx = (x1-12) / 24;
	  ty = (y1-25) / 18;
      DrawCkSquare(tx,ty,1);
      DrawCkSquare(tx-1,ty,1);
      DrawCkSquare(tx+1,ty+1,1);
      DrawCkSquare(tx+1,ty,1);
      DrawCkSquare(tx,ty+1,1);
      DrawCkSquare(tx-1,ty-1,1);
      DrawCkSquare(tx+1,ty-1,1);
      DrawCkSquare(tx,ty-1,1);
      BlitAnimate(m+i,x1-12,y1-9,1,notSrcBic);
      BlitAnimate(m,x1-12,y1-9,1,srcOr);
      LoadPartial();
	  x1 += dx * 4;
	  y1 += dy * 3;
	  SetMarH(Level,tx+MapOrigin.x,ty+MapOrigin.y,2);
      } while(((abs(x1-x2) > 2) || (abs(y1-y2) > 2)) &&
	   (x1 >= -8) && (x1 <= 239) &&
	   (y1 >= 0) && (y1 <= 182));
   SetMarH(Level,x,y,2);
   SetMarH(Level,MapOrigin.x+4,MapOrigin.y+4,0);
   SoundEff(-1);
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    Damage(4,Level,(int)i,j);
   DrawMapWindow();
   if(MapWindow != NULL)
    if(Quest->marine[tnum].condition > 0) PrintLineXY("hit.",235,85);
    else PrintLineXY("killed.",235,85);
   Quest->opponent[CurEnemy].encumbrance -= 5;
   CurMove -= 5;
   }
   
   

EnemyGunFire(etype)
int etype;

/* Enemy fires a gun */

{
   int dist,i,f,accu,e;
   char astr[25];
   
   dist = trng;
   TurnToFace();
   DrawMapWindow();
   FindTarget(targx,targy,4,&targx,&targy);
   if(trng <= 4) {
      /* do gun special effect */
      SetColor(2);
	  f = Quest->opponent[CurEnemy].facing;
      switch(etype) {
         case 2: SoundEff(3);
		         MoveTo(maragun[(f << 1)],maragun[(f << 1)+1]);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 3: SetColor(5);
		         SoundEff(6);
		         MoveTo(aliengun[(f << 1)]+96,aliengun[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 5: SoundEff(4);
		         MoveTo(gungun1[(f << 1)]+96,gungun1[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 MoveTo(gungun2[(f << 1)]+96,gungun2[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 7: SoundEff(3);
		         MoveTo(tankgun[(f << 1)]+96,tankgun[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 8: SoundEff(5);
		         e = Quest->map[Level][curx][cury];
		         DrawMasked((328+f*2)+16,4,4,e,0);
		         MoveTo(robotgun1[(f << 1)]+96,robotgun1[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 MoveTo(robotgun2[(f << 1)]+96,robotgun2[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 9: SoundEff(3);
		         MoveTo(bipedgun1[(f << 1)]+96,bipedgun1[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 MoveTo(bipedgun2[(f << 1)]+96,bipedgun2[(f << 1)+1]+88);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         case 10: SoundEff(3);
		         MoveTo(108,94);
                 DrawTo((targx-MapOrigin.x)*24+12,(targy-MapOrigin.y)*18+25);
                 break;
         }
      Delay(3L);
      DrawMapWindow();
      if(FindMarine(targx,targy) != 10) {
         /* Draw gun text in window */
         ClearMstats();
         if(etype == 3) PrintLineXY("Eye Beams:",235,65);
		 else PrintLineXY("Laser Fire:",235,65);
         strptoc(astr,Quest->marine[tnum].name);
         astr[12] = 0;
         PrintLineXY(astr,235,75);
		 accu = Quest->opponent[CurEnemy].shoot;
		 if(Quest->map[Level][curx][cury] == 83) accu += 5;
         if(Rnd(100) <= accu) Zap();
         else PrintLineXY("missed.",235,85);
         Delay(cdel);
         }
      CurMove -= 3;
      }
   }
   
   
   
FireSmokeGrenade(x,y)
int x,y;

{
   int j,i;
   
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    if((i >= 0) && (j >= 0) && (i <= 49) && (j <= 49))
    SetMarH(Level,i,j,2);
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   }
   
   

FireGrenade(x,y)
int x,y;

{
   int j,i;
   
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    Damage(4,Level,(int)i,j);
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   }
   
   

EnemyGrenadeFire()

/* Enemy fires a grenade */

{
   char astr[25];

   DrawMapWindow();
   ClearMstats();
   PrintLineXY("Grenade:",235,65);
   strptoc(astr,Quest->marine[tnum].name);
   astr[12] = 0;
   PrintLineXY(astr,235,75);
   FireGrenade(targx,targy);
   if(MapWindow != NULL)
    if(Quest->marine[tnum].condition > 0) PrintLineXY("hit.",235,85);
    else PrintLineXY("killed.",235,85);
   Quest->opponent[CurEnemy].encumbrance -= 5;
   CurMove -= 5;
   }
   
   
   
EnemySmokeGrenade()

/* Enemy fires a smoke grenade */

{
   char astr[25];

   DrawMapWindow();
   ClearMstats();
   PrintLineXY("Smoke Grenade:",235,65);
   strptoc(astr,Quest->marine[tnum].name);
   astr[12] = 0;
   PrintLineXY(astr,235,75);
   FireSmokeGrenade(targx,targy);
   if(MapWindow != NULL)
    if(Quest->marine[tnum].condition > 0) PrintLineXY("hit.",235,85);
    else PrintLineXY("killed.",235,85);
   Quest->opponent[CurEnemy].encumbrance -= 5;
   CurMove -= 5;
   }
   
   
   
EnemyPsionicFire()

/* Enemy fires a psionic */

{
   int x,y;
   char astr[25];
   
   DrawMapWindow();
   x = Quest->marine[tnum].x - MapOrigin.x;
   y = Quest->marine[tnum].y - MapOrigin.y;
   ClearMstats();
   PrintLineXY("Psionic:",235,65);
   strptoc(astr,Quest->marine[tnum].name);
   astr[12] = 0;
   PrintLineXY(astr,235,75);
   if(Rnd(100) <= (Quest->opponent[CurEnemy].vitality / 2)) {
      SoundEff(2);
	  PrintLineXY("stunned.",235,85);
      BlitCreature(99,x,y,0,notSrcBic);
      BlitCreature(98,x,y,0,srcOr);
      Quest->marine[tnum].vitality = 0;
      }
   else {
	  PrintLineXY("failed.",235,85);
      Quest->opponent[CurEnemy].vitality = 50;
      }
   CurMove = 0;
   }
   
   
   
EnemyMarineAttack()

/* Do the attacks for the enemy marine */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      if((Quest->opponent[CurEnemy].armor <= 2) && (Rnd(4) > 1))
       EnemyGunFire(Quest->opponent[CurEnemy].type);
      else if((Quest->opponent[CurEnemy].encumbrance >= 10) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 70))
       EnemyRocketFire();
      else if((Quest->opponent[CurEnemy].encumbrance >= 1) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 30) &&
	   (GetMarH(Level,targx,targy) == 0))
       EnemySmokeGrenade();
      else if((Quest->opponent[CurEnemy].encumbrance >= 1) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 90))
       EnemyGrenadeFire();
      else if((Quest->opponent[CurEnemy].psi) &&
       (Rnd(100) < 50)) EnemyPsionicFire();
      else EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyAlienAttack()

/* Do the attacks for the enemy alien */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      if((Quest->opponent[CurEnemy].armor <= 2) && (Rnd(4) > 1))
       EnemyGunFire(Quest->opponent[CurEnemy].type);
      else if((Quest->opponent[CurEnemy].encumbrance >= 10) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 70))
       EnemyRocketFire();
      else if((Quest->opponent[CurEnemy].encumbrance >= 1) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 30) &&
	   (GetMarH(Level,targx,targy) == 0))
       EnemySmokeGrenade();
      else if((Quest->opponent[CurEnemy].encumbrance >= 1) && 
       (CurMove >= 5) && (trng > 1) && (Rnd(100) < 90))
       EnemyGrenadeFire();
      else if((Quest->opponent[CurEnemy].psi) &&
       (Rnd(100) < 50)) EnemyPsionicFire();
      else EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemySeekerAttack()

/* Do the attacks for the enemy seeker */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      if((Quest->opponent[CurEnemy].armor <= 2) && (Rnd(4) > 1))
       EnemyGunFire(Quest->opponent[CurEnemy].type);
      else if((Quest->opponent[CurEnemy].psi) &&
       (Rnd(100) < 50)) EnemyPsionicFire();
      else EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyGunAttack()

/* Do the attacks for the enemy gun emplacement */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyBipedAttack()

/* Do the attacks for the enemy biped */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyRobotAttack()

/* Do the attacks for the enemy robot */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      SoundEff(16);
      EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyTankAttack()

/* Do the attacks for the enemy tank */

{
   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0)) {
      EnemyGunFire(Quest->opponent[CurEnemy].type);
      Delay(cdel);
      }
   else CurMove = 0;
   }
   
   

EnemyBeastAttack()

/* Do the attacks for the beast */

{
   char astr[25];
   int e,f;

   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0) && (trng == 1)) {
      ClearMstats();
      PrintLineXY("Beast:",235,65);
      strptoc(astr,Quest->marine[tnum].name);
      astr[12] = 0;
      PrintLineXY(astr,235,75);
      SoundEff(1);
      TurnToFace();
      /* Do beast biting effect */
      DrawMapWindow();
	  obs[Level][curx][cury] = 0;
	  DrawAltMap();
	  obs[Level][curx][cury] = 101;
      Delay(15L);
      e = Quest->map[Level][curx][cury];
	  f = Quest->opponent[CurEnemy].facing;
	  DrawMasked((184+f*2)+16,4,4,e,1);
      LoadPartial();
      /* Update the marine window */
      if(Rnd(100) <= Quest->opponent[CurEnemy].shoot) {
         PrintLineXY("bitten.",235,85);
         Damage(1,Level,targx,targy);
         }
      else PrintLineXY("missed.",235,85);
      Delay(cdel);
      CurMove -= 5;
      }
   else outrng = TRUE;
   }
   
   

EnemyTerradonAttack()

/* Do the attacks for the terradon */

{
   char astr[25];
   int e,f;

   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0) && (trng == 1)) {
      ClearMstats();
      PrintLineXY("Teradon:",235,65);
      strptoc(astr,Quest->marine[tnum].name);
      astr[12] = 0;
      PrintLineXY(astr,235,75);
      SoundEff(8);
      TurnToFace();
      /* Do beast biting effect */
      DrawMapWindow();
	  obs[Level][curx][cury] = 0;
	  DrawAltMap();
	  obs[Level][curx][cury] = 104;
      Delay(15L);
      e = Quest->map[Level][curx][cury];
	  f = Quest->opponent[CurEnemy].facing;
	  DrawMasked((248+f*2)+16,4,4,e,1);
      LoadPartial();
      /* Update the marine window */
      if(Rnd(100) <= Quest->opponent[CurEnemy].shoot) {
         PrintLineXY("bitten.",235,85);
         Damage(1,Level,targx,targy);
         }
      else PrintLineXY("missed.",235,85);
      Delay(cdel);
      CurMove -= 5;
      }
   else outrng = TRUE;
   }
   
   

QuillLine(x,y,dx,dy)
int x,y,dx,dy;

{
   MoveTo(x,y);
   DrawTo(x+dx,y+dy);
   }
   
   
   
DrawQuills(qnum,x,y,dx,dy,sx,sy,dir)
int qnum,x,y,dx,dy,sx,sy,dir;

{
   int z,px,py;
   
   SetColor(0);
   switch(qnum) {
      case 0: QuillLine(x,y,dx,dy);
			  break;
	  case 1: QuillLine(x,y,dx,dy);
	          for(z = 0; z < 2; z++) 
	           QuillLine(x + (quills2[dir][z << 1]*sx),
			   y + (quills2[dir][(z << 1)+1]*sy),dx,dy);
			  break; 
	  case 2: QuillLine(x,y,dx,dy);
	          for(z = 0; z < 2; z++) 
	           QuillLine(x + (quills2[dir][z << 1]*sx),
			   y + (quills2[dir][(z << 1)+1]*sy),dx,dy);
	          for(z = 0; z < 3; z++) 
	           QuillLine(x + (quills3[dir][z << 1]*sx),
			   y + (quills3[dir][(z << 1)+1]*sy),dx,dy);
			  break; 
	  default: for(z = 0; z < 7; z++) 
	           QuillLine(x + (quills4[dir][z << 1]*sx),
			   y + (quills4[dir][(z << 1)+1]*sy),dx,dy);
			  break; 
	  }	  		  
   }
   
   
   
EnemyWadgelAttack()

/* Do the attacks for the wadgel */

{
   char astr[25];
   int x,f,dx,dy,sx,sy;

   MarineCrossTarget(&tnum,&trng,&tdir);
   if((tnum != -1) && (CurMove > 0) && (trng == 1)) {
      ClearMstats();
      PrintLineXY("Wadgel:",235,65);
      strptoc(astr,Quest->marine[tnum].name);
      astr[12] = 0;
      PrintLineXY(astr,235,75);
      TurnToFace();
	  SoundEff(7);
	  f = Quest->opponent[CurEnemy].facing;
      switch(f) {
         case 0: sx = 0; sy = -1; break;
         case 1: sx = 1; sy = -1; break;
         case 2: sx = 1; sy = 0; break;
         case 3: sx = 1; sy = 1; break;
         case 4: sx = 0; sy = 1; break;
         case 5: sx = -1; sy = 1; break;
         case 6: sx = -1; sy = 0; break;
         case 7: sx = -1; sy = -1; break;
   	     }
	  dx = sx * 2;
	  dy = sy * 2;
	  sy = -sy;
	  if(sx == 0) sx = 1;
	  if(sy == 0) sy = 1;
	  if(f > 2) {
	     if(f == 4) f = 0;
		 else if(f == 6) f = 2;
		 else f = 1;
		 }
      /* Do beast biting effect */
      DrawMapWindow();
	  SavePartial();
      Delay(15L);
	  SetColor(0);
	  for(x = 0; x < 9; x++) {
	     DrawSquare(4,4,1);
		 LoadPartial();
		 DrawQuills(x,108+dx*x,97+dy*x,dx,dy,sx,sy,f);
		 Delay(1L);
		 }
	  LoadPartial();
      /* Update the marine window */
      if(Rnd(100) <= Quest->opponent[CurEnemy].shoot) {
         PrintLineXY("stabbed.",235,85);
         Damage(1,Level,targx,targy);
         }
      else PrintLineXY("missed.",235,85);
      Delay(cdel);
      CurMove -= 5;
      }
   else outrng = TRUE;
   }
   
   

EnemyAttack()

/* Pick the type of enemy attack */

{
   switch(Quest->opponent[CurEnemy].type) {
      case 1: EnemyBeastAttack();
              break;
      case 2: EnemyMarineAttack();
              break;
      case 3: EnemyAlienAttack();
              break;
	  case 4: EnemyTerradonAttack();
	          break;
	  case 5: EnemyGunAttack();
	          break;
	  case 6: EnemyWadgelAttack();
	          break;
	  case 7: EnemyTankAttack();
	          break;
	  case 8: EnemyRobotAttack();
	          break;
	  case 9: EnemyBipedAttack();
	          break;
	  case 10: EnemySeekerAttack();
	          break;
      default: CurMove--;
               break;
      }
   }

