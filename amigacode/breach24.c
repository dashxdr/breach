/* breach24.c - Icons */

#include   "breach2.h"

/* External Procedures */
extern Boolean PtInRect();
extern Boolean CheckMove();
extern Boolean SlashWall();
extern Boolean BackslashWall();

/* External Variables */
extern Byte movc[];  
extern Byte wallmoves[];
extern Boolean Speedy[],Invis[];
extern struct QstStru *Quest;
extern Byte obs[5][50][50],Heater[10];
extern Byte mar[5][50][50];
extern Boolean NewFlag,Changed,BeltFlag,SkipIt;
extern int MapWindow;
extern Point MapOrigin;
extern int Level;
extern Point MousePos;
extern Boolean modifier,optioned;
extern int IconFlag;
extern int CurMarine,CurMove;
extern int PointFlag,ArmFlag;
extern int Movements[10];
extern Byte passer[];
extern int NameCount;
extern int mnum[15],mtyp[15];
extern int tpath[4][4][4][2];
extern char *obna[];
extern Byte objenc[];
extern int CurItem,Start,theItem;
extern Boolean DropFlag,LaunchFlag;
extern int guns[];
extern int suits[];
extern int CurIndex;
extern int DemoLevel;
extern char *namer[];
extern Byte ElmName[];
extern Byte see[17][17]; 


Boolean PtInCheck(pt,x,y)
Point *pt;
int x,y;

{
   if(((*pt).x >= x) && ((*pt).x <= x+12) &&
    ((*pt).y >= y) && ((*pt).y <= y+9)) return(TRUE);
   return(FALSE);
   }
   
   
   
Boolean GotShield(mar)
int mar;

{
   int y;
   
   for(y = 0; y < 15; y++) if(GetType(mar,y) == 17) return(TRUE);
   return(FALSE);
   }
   
   
   
int abs(x)
int x;

{
   if(x < 0) return(-x);
   return(x);
   }
   
   
   
int FindObject(x,y)
int x,y;

/* Locates the index of the selected object */

{
   int z = -1;
   
   do {
      z++;
      } while(((Quest->object[z].x != x) ||
       (Quest->object[z].y != y) ||
       (Quest->object[z].level != Level) ||
       (Quest->object[z].held == TRUE) ||
       (Quest->object[z].type == 0)) && (z < numobj));
  return(z);
  }
   


int GetType(marnum,index)
int index,marnum;

/* Get the element type of the supplied index in the list box */

{
   int i;
   
   i = Quest->marine[marnum].inventory[index] - 1;
   if(i < 0) return(0);
   return(Quest->object[i].type);
   }
   


int FindMarine(x,y)
int x,y;

{
   int z = -1;

   if(GetMarL(Level,x,y) == 15) return(10);
   do {
       z++;
       } while(((Quest->marine[z].x != x) || (Quest->marine[z].y != y)
        || (Quest->marine[z].level != Level) || (Quest->marine[z].class
        == 0)) && (z < 10));
   return(z);
   }



int FindEnemy(x,y)
int x,y;

{
   int z = -1;

   do {
       z++;
       } while(((Quest->opponent[z].x != x) || (Quest->opponent[z].y != y)
        || (Quest->opponent[z].health == 0)
        || (Quest->opponent[z].level != Level) || (Quest->opponent[z].type
        == 0)) && (z < numopp));
   return(z);
   }



KillMarine(t)
int t;

{
   int x,y,i;
   
   x = Quest->marine[t].x - MapOrigin.x;
   y = Quest->marine[t].y - MapOrigin.y;
   if((x >= 0) && (x <= 8) && (y >= 0) && (y <= 8)) {
      BlitElement(95,x,y,0,notSrcBic);
      BlitElement(94,x,y,0,srcOr);
      BlitElement(97,x,y,0,notSrcBic);
      BlitElement(96,x,y,0,srcOr);
      }
   SoundEff(14); 
   Quest->marine[t].class = 0;
   Quest->marine[t].condition = 0;
   if(CurMarine == t) CurMove = 0;
   Movements[t] = 0;
   SetMarL(Level,Quest->marine[t].x,Quest->marine[t].y,15);
   Quest->lost++;
   for(x = 0; x <= 14; x++) {
      i = Quest->marine[t].inventory[x] - 1;
      if(i >= 0) {
         y = Quest->object[i].type;
		 Quest->object[i].type = 0;
         if((y == 9) || (y == 11)) EndGame(y);
		 }
      }
   }
   
   
   
KillNEEnemy(t)
int t;

{
   int x;
   
   x = Quest->opponent[t].type;
   if((x == 2) || (x == 3)) SoundEff(14);
   Quest->opponent[t].health = 0;
   obs[Level][Quest->opponent[t].x][Quest->opponent[t].y] = 0;
   Quest->kills++;
   }
   
   
   
KillEnemy(t)
int t;

{
   int x,y;
   
   x = Quest->opponent[t].x - MapOrigin.x;
   y = Quest->opponent[t].y - MapOrigin.y;
   if((x >= 0) && (x <= 8) && (y >= 0) && (y <= 8)) {
      BlitElement(95,x,y,0,notSrcBic);
      BlitElement(94,x,y,0,srcOr);
      BlitElement(97,x,y,0,notSrcBic);
      BlitElement(96,x,y,0,srcOr);
      }
   KillNEEnemy(t);
   }
   
   
   
KillText(lev,x,y)
int lev,x,y;

{
   int z;

   for(z = 0; z < 20; z++) if((Quest->ttext[z].level == lev) &&
    (Quest->ttext[z].y == y) && (Quest->ttext[z].x == x))
    Quest->ttext[z].read = TRUE;
   }



Damage(weap,lev,x,y)
int weap,lev,x,y;

{
   int i,n,m,t,el,dam,z,typ;
   
   if(weap == 0) z = 98;
   else z = 94;
   el = Quest->map[lev][x][y];
   
   if((x >= 0) && (x <= 49) && (y >= 0) && (y <= 49)) {
   
      /* Check for tank chain reaction explosions! */
      if((el >= 64) && (el <= 66) && (weap < 6) && (weap > 0)) {
         Quest->map[lev][x][y] = 87;
         KillText(lev,x,y);
         Damage(5,lev,x+1,y);
         Damage(5,lev,x-1,y);
         Damage(5,lev,x,y+1);
         Damage(5,lev,x,y-1);
         if((abs(MapOrigin.x+4 - x) <= 4) && (abs(MapOrigin.y+4 - y) <= 4)) {
            BlitCreature(97,x-MapOrigin.x,y-MapOrigin.y,0,notSrcBic);
            BlitCreature(96,x-MapOrigin.x,y-MapOrigin.y,0,srcOr);
            }
         }
         
      /* Check for barney rubbling of terrain */
      if(weap == 4) {
		 t = 0;
		 if((el >= 26) && (el <= 33)) t = 87;
		 else if((el >= 36) && (el <= 69) && (el != 63)) t = 87;
		 else if((el >= 71) && (el <= 72)) t = 87;
		 else if((el >= 85) && (el <= 88) && (el != 87)) t = 84;
         if(t != 0) {
		    Quest->map[lev][x][y] = t;
		    KillText(lev,x,y);
            }
         }
      
      if(weap == 5) {
		 t = 0;
		 if((el >= 18) && (el <= 33)) t = 87;
		 else if((el == 34) || (el == 35)) t = 87;
		 else if((el >= 36) && (el <= 69)) t = 87;
		 else if((el >= 71) && (el <= 72)) t = 87;
		 else if((el >= 85) && (el <= 88) && (el != 87)) t = 84;
         if(t != 0) {
		    Quest->map[lev][x][y] = t;
		    KillText(lev,x,y);
            }
         }
      
      /* Check for destruction of objects */
      if((weap >= 2) && (weap <= 5) && (obs[lev][x][y] > 0) && 
       (obs[lev][x][y] < 100)) {
         if((obs[lev][x][y] == 9) || (obs[lev][x][y] == 11)) {
		    if(obs[lev][x][y] == 11) SoundEff(14);
            if((abs(MapOrigin.x+4 - x) <= 4) && (abs(MapOrigin.y+4 - y) <= 4)) {
               BlitCreature(97,x-MapOrigin.x,y-MapOrigin.y,0,notSrcBic);
               BlitCreature(96,x-MapOrigin.x,y-MapOrigin.y,0,srcOr);
			   }
            EndGame(obs[lev][x][y]);
			}
         Quest->object[FindObject(x,y)].type = 0;
         obs[lev][x][y] = 0;
         }
      else if((weap == 6) && (obs[lev][x][y] == 11)) { 
         EndGame(obs[lev][x][y]);
         Quest->object[FindObject(x,y)].type = 0;
         obs[lev][x][y] = 0;
         }
         
      /* Check for damage to player characters */
      t = FindMarine(x,y);
      if(t < 10) {
         if((abs(MapOrigin.x+4 - x) <= 4) && (abs(MapOrigin.y+4 - y) <= 4)) {
            BlitElement(z+1,x-MapOrigin.x,y-MapOrigin.y,0,notSrcBic);
            BlitElement(z,x-MapOrigin.x,y-MapOrigin.y,0,srcOr);
            }
		 if(weap == 0) Quest->marine[t].vitality = 0;
		 dam = guns[weap] - suits[Quest->marine[t].armor - 1];
		 if(GotShield(t)) dam = (dam * 8) / 10;
		 if(el == 86) dam = (dam * 8) / 10;
         if(dam > 0) Quest->marine[t].condition -= dam;
         if(Rnd(100) > (Quest->marine[t].condition * 2))
          Quest->marine[t].condition = 0;
         if(Quest->marine[t].condition <= 0) KillMarine(t);
         }
      
      /* Check for damage to opponents */
      i = FindEnemy(x,y);
      if(i < numopp) {
	     typ = Quest->opponent[i].type;
         if((abs(MapOrigin.x+4 - x) <= 4) && (abs(MapOrigin.y+4 - y) <= 4)) {
            BlitElement(z+1,x-MapOrigin.x,y-MapOrigin.y,0,notSrcBic);
            BlitElement(z,x-MapOrigin.x,y-MapOrigin.y,0,srcOr);
            }
		 if((weap == 0) && (typ <= 4)) Quest->opponent[i].vitality = 0;
		 dam = guns[weap] - suits[Quest->opponent[i].armor - 1];
		 if(el == 86) dam *= 0.8;
		 if((weap == 6) && (typ >= 5) && (typ != 6) && (typ != 7)) dam = 0;
		 else {
		    if(dam > 0) Quest->opponent[i].health -= dam; 
            if((Rnd(100) > (Quest->opponent[i].health * 2)) ||
             (Rnd(100) <= 5)) Quest->opponent[i].health = 0;
			}
         if(Quest->opponent[i].health <= 0) {
		    if((typ == 10) && ((weap <= 4) || (weap == 6))) {
			   Quest->opponent[i].vitality = 105;
			   Quest->opponent[i].health = 100;
			   }
		    else {
               if((CurMarine == 9) && (weap < 3)) Quest->slkills++;
			   KillEnemy(i);
			   }
			}
         }
      }
   }
            
         

int StopSquare(x,y,px,py,i,mc)
int x,y,px,py,i;
Boolean mc;

{
   int ob,e,dx,dy,e1,e2,l,dir;

   if(i == 0) return(0);
   l = Level;
   dx = x - px;
   dy = y - py;
   e = Quest->map[Level][x][y];
   if((dx != 0) && (dy != 0)) {
      if(dx < 0) dir = 6 - dy;
      else if(dx > 0) dir = 2 + dy;
      else {
         if(dy < 0) dir = 0;
   	     else dir = 4;
	     }
      if(dir == 1) {
	     e1 = Quest->map[l][x-1][y];
		 e2 = Quest->map[l][x][y+1];
		 }
      if(dir == 3) {
	     e1 = Quest->map[l][x][y-1];
		 e2 = Quest->map[l][x-1][y];
		 }
      if(dir == 5) {
	     e1 = Quest->map[l][x+1][y];
		 e2 = Quest->map[l][x][y-1];
		 }
      if(dir == 7) {
	     e1 = Quest->map[l][x+1][y];
		 e2 = Quest->map[l][x][y+1];
		 }
	  if(((dir == 1) || (dir == 5)) && (BackslashWall(e1) ||
	   BackslashWall(e2))) {
	     return(2);
		 }
	  if(((dir == 3) || (dir == 7)) && (SlashWall(e1) ||
	   SlashWall(e2))) return(2);
	  }
   else if(i == 1) {
      if(dx > 0) dir = 2;
	  if(dx < 0) dir = 6;
	  if(dy > 0) dir = 4;
	  if(dy < 0) dir = 0;
      e1 = Quest->map[Level][px][py];
      if(e1 <= 52) {
         if(wallmoves[e1-18] & (0x01 << dir)) return(2);
   	     }
      }
   if(passer[e] == 0) return(1);
   if(e < 48) return(1);
   if((x-MapOrigin.x == 4) && (y-MapOrigin.y == 4)) return(0);
   ob = obs[Level][x][y];
   if(ob > 100) return(1);
   if(mc && (FindMarine(x,y) != 10)) return(1);
   return(0);
   }



FindTarget(x2,y2,range,ex,ey)
int x2,y2,range;
int *ex,*ey;

{
   int x1,y1,dx,dy,d,i,px,py,z;
   int incr1,incr2,x,y,ox = 1,oy = 1,sx,sy;
   Boolean str = FALSE;
   
   x1 = MapOrigin.x+4;
   y1 = MapOrigin.y+4;
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   if(x2 < x1) ox = -1;
   if(y2 < y1) oy = -1;
   if((dx == 0) || (dy == 0) || (dx == dy)) {
      str = TRUE;
      if(dx == 0) ox = 0;
      if(dy == 0) oy = 0;
      }
   x = x1;
   y = y1;
   px = x;
   py = y;
   for(i = 0; i < range+1; i++) {
      if(range == 50) z = StopSquare(x,y,px,py,i,TRUE);
      else z = StopSquare(x,y,px,py,i,FALSE);
	  if(z == 1) break;
	  if(z == 2) {
	     x = px;
		 y = py;
		 }
	  if(z == 2) break;
	  if(i == range) break;
      if((x == x2) && (y == y2)) break;
	  px = x;
	  py = y;
      if(str) {
         x += ox;
         y += oy;
         }
      else if(i < 4) {         
         sx = tpath[dx-1][dy-1][i][0]; 
         sy = tpath[dx-1][dy-1][i][1];
         if(sx != 0) x += ox;
         if(sy != 0) y += oy;
         }
      if(x < 0) x = 0;
      if(x > 49) x = 49;
      if(y < 0) y = 0;
      if(y > 49) y = 49;
      }
   *ex = x;
   *ey = y;
   }



UpdateMarine()

{
   DrawMarineWindow();
   }
 


SetMarList()

{
   int z = 0,inum;

   NameCount = 0;
   do {
      inum = Quest->marine[CurMarine].inventory[z];
      if(inum != 0) {
         NameCount++;
         inum = Quest->object[inum-1].type;
         if(inum == 13) LaunchFlag = TRUE;
         mnum[NameCount-1] = z;
         mtyp[NameCount-1] = inum;
         }
      z++;
      } while(z < 15);
   }



DoCrackUnit()

/* Use a crack unit */

{
   int x,y,elm;
   Boolean adj;
   
   /* Check for adjacent terminal or computer */
   adj = FALSE;
   for(x = MapOrigin.x+3; x <= MapOrigin.x+5; x++)
    for(y = MapOrigin.y+3;y <= MapOrigin.y+5; y++)
	if((x >= 0) && (x <= 49) && (y >= 0) && (y <= 49)) {
      elm = Quest->map[Level][x][y];
  	  if((elm == 60) || (elm == 67)	|| (elm == 57)) adj = TRUE;
	  }
   if(!adj) IconMessage(18);
   else {
      CurMove -= 9;
      Quest->marine[CurMarine].vitality -= 9;
      if(Rnd(100) < Quest->marine[CurMarine].crack) {
         if(CurMarine == 9) Quest->slcracks++;
         for(x = 0; x < 50; x++) for(y = 0; y < 50; y++) {
            elm = Quest->map[Level][x][y];
            if((elm <= 33) || ((elm >= 36) && (elm <= 53)))
             BitSet(&Quest->vis[x][y],(long)Level);
            }
         }
      else IconMessage(19);
      DoWhole();
      DrawMapWindow();
      }
   }
   
   
   
DoDetector()

/* Use a detector */

{
   int x,y,t;
   
   CurMove -= 15;
   t = Quest->marine[CurMarine].class;
   Quest->marine[CurMarine].vitality -= 15;
   if(Rnd(100) < Quest->marine[CurMarine].detect) {
      for(x = MapOrigin.x-6; x <= MapOrigin.x + 14; x++)
       for(y = MapOrigin.y-6; y <= MapOrigin.y + 14; y++)
       if((x >= 0) && (x <= 49) && (y >= 0) && (y <= 49))
       BitSet(&Quest->vis[x][y],(long)Level);
      if(CurMarine == 9) Quest->sldetects++;
      Quest->marine[CurMarine].class = 3;
      }
   else IconMessage(38);
   DoScale();
   Quest->marine[CurMarine].class = t;
   DrawMapWindow();
   }
   
   
   
DoRocket()

{
   int Rx,x,y,i,j,x1,y1,x2,y2,p,s1,f,dx,dy,tx,ty,m;
   Point p2,p1;

   SoundEff(10);
   x = MapOrigin.x + 4;
   y = MapOrigin.y + 4;
   f = Quest->marine[CurMarine].facing;
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
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   DeleteItem();
   CurMove -= 5;
   Quest->marine[CurMarine].vitality -= 5;
   }
   
   

DoDemoCharge(index)
int index;

/* Arm a demo charge */

{
   CurMove -= 10;
   Quest->marine[CurMarine].vitality -= 10;
   Quest->object[index].type = 51;
   DropIt();
   }
   
   

DetonateDemo()

/* Detonate the remote charges */

{
   int x,y,i,j,tl,z,l;
   Point tp;

  if(YesNoBox(65,FALSE) != 1) return;
   for(z = 0; z < numobj; z++) if(Quest->object[z].type == 51) {
      x = Quest->object[z].x;
      y = Quest->object[z].y;
	  l = Quest->object[z].level;
      tp = MapOrigin;
	  tl = Level;
      Level = l;
      SetPt(&MapOrigin,x-4,y-4);
      DrawMapWindow();
	  SoundEff(9);
	  DoExplosion(x,y,2);
	  PageFlip(0);
      /* Redraw the map with the rubble in place */
      for(i = x-2; i <= x+2; i++) for(j = y-2; j <= y+2; j++)
       Damage(5,Level,i,j);
      DrawMapWindow();
	  Quest->object[z].type = 0;
      Level = tl;
      MapOrigin = tp;
	  }
   DrawMapWindow();
   DrawMarineWindow();
   }
   
   

DetonateProx(x,y)
int x,y;

/* Detonate the proximity charges */

{
   int i,j,tl,z,l;
   Point tp;
   
   z = FindObject(x,y);
   if(z == numobj) return;
   x = Quest->object[z].x;
   y = Quest->object[z].y;
   tp = MapOrigin;
   SetPt(&MapOrigin,x-4,y-4);
   DrawMapWindow();
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   /* Redraw the map with the rubble in place */
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    Damage(4,Level,i,j);
   Quest->object[z].type = 0;
   DrawMapWindow();
   MapOrigin = tp;
   }
   
   

DoGrenade(x1,y1)
int x1,y1;

{
   int x,y,j,i;

   CurMove -= 5;
   Quest->marine[CurMarine].vitality -= 5;
   FindTarget(x1+MapOrigin.x,y1+MapOrigin.y,3,&x,&y);
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    Damage(4,Level,(int)i,j);
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   DeleteItem();
   }
   
   

DoSmokeGrenade(x1,y1)
int x1,y1;

{
   int x,y,j,i;

   CurMove -= 5;
   Quest->marine[CurMarine].vitality -= 5;
   FindTarget(x1+MapOrigin.x,y1+MapOrigin.y,3,&x,&y);
   SoundEff(9);
   DoExplosion(x,y,1);
   PageFlip(0);
   for(i = x-1; i <= x+1; i++) for(j = y-1; j <= y+1; j++)
    if((i >= 0) && (j >= 0) && (i <= 49) && (j <= 49))
    SetMarH(Level,i,j,2);
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   DeleteItem();
   }
   
   

DoNeutronBomb(x1,y1)
int x1,y1;

{
   int x,y,j,i;

   CurMove -= 6;
   Quest->marine[CurMarine].vitality -= 6;
   FindTarget(x1+MapOrigin.x,y1+MapOrigin.y,3,&x,&y);
   SoundEff(17);
   DoExplosion(x,y,2);
   PageFlip(0);
   for(i = x-2; i <= x+2; i++) for(j = y-2; j <= y+2; j++)
    Damage(6,Level,(int)i,j);
   /* Redraw the map with the rubble in place */
   DrawMapWindow();
   DeleteItem();
   }
   
   

DoExplosion(x,y,r)
int x,y,r;

{
   int i;
   Point p1;

   r = r * 13 + 22;
   if((x < MapOrigin.x) || (y < MapOrigin.y) || 
    (y > MapOrigin.y+8) || (x > MapOrigin.x+8)) return;
   SetPt(&p1,(x-MapOrigin.x)*24+12,(y-MapOrigin.y)*18+25);
   SaveBuffer();
   PageFlip(1);
   SetColor(5);
   for(i = 0; i < r; i+=3) {
       DoCircle(p1.x,p1.y,i);
       SavePartial();
       }
   LoadBuffer();
   }
   
   

DeleteItem()

{
   int i;
   
   i = Quest->marine[CurMarine].inventory[CurIndex] - 1; 
   Quest->marine[CurMarine].encumbrance -= 
    objenc[(Quest->object[i].type - 9) / 2];
   Quest->object[i].type = 0;
   Quest->marine[CurMarine].inventory[CurIndex] = 0;
   }
   
   
   
DoAmmoPack()

/* Load an ammo pack */

{
   int i;
   
   SoundEff(11);
   CurMove -= 8;
   Quest->marine[CurMarine].vitality -= 8;
   Quest->marine[CurMarine].ammo += 50;
   DeleteItem();
   }
   
   
   
DoMedikit()

/* Use a medikit */

{
   int i;
   
   CurMove -= 8;
   if(Rnd(30) > Quest->marine[CurMarine].condition)
    KillMarine(CurMarine);
   else {
      Quest->marine[CurMarine].vitality += 50;
      if(Quest->marine[CurMarine].vitality > 100)
       Quest->marine[CurMarine].vitality = 100;
      Quest->marine[CurMarine].condition += 50;
      if(Quest->marine[CurMarine].condition > 100)
       Quest->marine[CurMarine].condition = 100;
	  }
   DeleteItem();
   DrawListIcon();
   }
   
   
   
DoTablet()

/* Use a tablet */

{
   int i;
   
   CurMove -= 16;
   Speedy[CurMarine] = TRUE;
   DeleteItem();
   }
   
   
   
DoGravBelt()

/* Use a gravbelt */

{
   CurMove -= 10;
   Quest->marine[CurMarine].vitality -= 10;
   BeltFlag = TRUE;
   DrawMapWindow();
   }
   
   
   
DigFoxhole()

{
   int elm;
   
   elm = Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4];
   if((elm == 83) || (elm == 84) || (elm == 85) || (elm == 88)) {
      CurMove -= 20;
      Quest->marine[CurMarine].vitality -= 20;
      Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4] = 86;
      DrawMapWindow();
	  }
   else IconMessage(63);
   }
   
   
   
DoItem()

{
   int x,y;
   
   x = MousePos.x / 24;
   y = (MousePos.y-16) / 18;
   switch(PointFlag) {
      case 15: DoSmokeGrenade(x,y);
                break;
      case 21: DoNeutronBomb(x,y);
                break;
      case 47: DoGrenade(x,y);
                break;
      }
   DrawMarineWindow();
   }
   
   

UseIt()

{
   int index,elm;
   
   theItem = 2;
   CurIndex = mnum[CurItem];
   index = Quest->marine[CurMarine].inventory[CurIndex] - 1;
   elm = Quest->object[index].type;
   switch(elm) {
      case 9: IconMessage(53); break;
	  case 11: IconMessage(54); break;
	  case 13: IconMessage(2); break;
      case 15: if(CurMove < 5) IconMessage(55);
               else {
                  PointFlag = 15;
                  MoveCursor(0);
                  ShapeCursor(4);
                  }
               break;
	  case 17: IconMessage(61); break;
      case 19: if(CurMove < 11) IconMessage(27);
                else DoGravBelt();
                break;
      case 21: if(CurMove < 6) IconMessage(12);
               else {
                  PointFlag = 21;
                  MoveCursor(0);
                  ShapeCursor(4);
                  }
               break;
	  case 23: IconMessage(60); break;
	  case 25: Quest->marine[CurMarine].gun = 1; break;
	  case 27: Quest->marine[CurMarine].gun = 2; break;
	  case 29: Quest->marine[CurMarine].gun = 0; break;
      case 31: if(CurMove < 9) IconMessage(17);
                else DoCrackUnit();
                break;
      case 33: if(CurMove < 15) IconMessage(4);
                else DoDetector();
                break;
      case 35: if(CurMove < 9) IconMessage(14);
               else DoMedikit();
               break;
      case 37: if(CurMove < 8) IconMessage(13);
               else DoAmmoPack();
               break;
	  case 39: IconMessage(66); break;
	  case 41: if(CurMove < 16) IconMessage(77);
	           else DoTablet();
			   break;
	  case 43: if(CurMove < 20) IconMessage(62);
	           else DigFoxhole();
			   break;
      case 45: if(CurMove < 5) IconMessage(15);
               else if(!LaunchFlag) IconMessage(16);
               else DoRocket();
               break;
      case 47: if(CurMove < 5) IconMessage(10);
               else {
                  PointFlag = 47;
                  MoveCursor(0);
                  ShapeCursor(4);
                  }
               break;
      case 49: if(CurMove < 10) IconMessage(11);
                else DoDemoCharge(index);
                break;
      }
   DrawMarineWindow();
   }


   
DoUseObject()

{
   int x,xu,done = 0;
   
   if(Quest->marine[CurMarine].armor == 4) LaunchFlag = TRUE;
   else LaunchFlag = FALSE;

   DrawItemBox();
   SetColor(12);
   Rectangle(105L,119L,155L,130L);
   PrintLineXY("USE",121,122);
   SetColor(15);
   RectangleFill(165,119,215,130);
   SetColor(12);
   Rectangle(165L,119L,215L,130L);
   PrintLineXY("CANCEL",172,122);
   Start = 0;
   DrawItemNames();
   do {
      x = mygetch();
	  if(x > 0) xu = toupper(x);
	  if(x == 2000) SelectUp();
	  else if(x == 2002) SelectDown();
	  else if(x == 2004) {
	     MousePos.y = 0;
		 DoItemScroll();
		 }
	  else if(x == 2005) {
	     MousePos.y = 199;
		 DoItemScroll();
		 }
	  else if(xu == 'C') done = 2;
	  else if(xu == 'U') done = 1;
	  if(x == -1) {
	     if(PtInRect(&MousePos,91,71,215,110)) 
		  SelectItem((MousePos.y - 71) / 8);
	     if(PtInRect(&MousePos,105,119,155,130)) done = 1;
	     if(PtInRect(&MousePos,165,119,215,130)) done = 2;
         if(PtInRect(&MousePos,216,71,228,111)) DoItemScroll();
		 }
	  } while(done == 0);
   FlashButton(done * 60 + 45,119,done * 60 + 95,130);
   LoadBuffer();
   if((CurItem != -1) && (done == 1)) {
      CurItem += Start;
	  UseIt();
	  }
   }



CheckDisarm()

{
   int y,lfor;
   
   lfor = Quest->marine[CurMarine].gun;
   if(lfor == -1) return;
   switch(lfor) {
      case -1: return; break;
      case 0: lfor = 29; break;
	  case 1: lfor = 25; break;
	  case 2: lfor = 27; break;
	  case 3: return; break;
	  }
   for(y = 0; y < 15; y++) if(GetType(CurMarine,y) == lfor) return;
   Quest->marine[CurMarine].gun = -1;
   }
   
   
   
SetInvis(mar)
int mar;

{
   int y;
   
   Invis[mar] = FALSE;
   for(y = 0; y < 15; y++) if(GetType(mar,y) == 23) Invis[mar] = TRUE;
   }
   
   
   
DropIt()

{
   int item,index,elm;
   
   theItem = 2;
   item = mnum[CurItem];
   index = Quest->marine[CurMarine].inventory[item] - 1;
   elm = Quest->object[index].type;
   Quest->marine[CurMarine].encumbrance -= objenc[(elm - 9) / 2];
   Quest->marine[CurMarine].inventory[item] = 0;
   Quest->object[index].held = FALSE;
   Quest->object[index].level = Level;
   Quest->object[index].x = MapOrigin.x + 4;
   Quest->object[index].y = MapOrigin.y + 4;
   obs[Level][MapOrigin.x+4][MapOrigin.y+4] = elm;
   CurMove--;
   SetInvis(CurMarine);
   CheckDisarm();
   DrawMapWindow();
   DrawMarineWindow();
   }
   
   

DrawItemNames()

{
   int x,r;
   
   SetColor(15);
   RectangleFill(91,71,215,110);
   x = Start;
   r = 0;
   while((x < NameCount) && (r < 5)) {
      PrintLineXY(obna[(mtyp[x]-9)/2],93,72+r*8);
	  x++;
	  r++;
	  }
   CurItem = -1;
   SelectDown();
   }
   
   

SelectUp()

{
   if(CurItem <= 0) return();
   InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   CurItem--;
   InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   }
   
   
   
SelectDown()

{
   if(CurItem >= 4) return();
   if(Start+CurItem+1 >= NameCount) return;
   if(CurItem != -1) InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   CurItem++;
   InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   }
   
   
   
SelectItem(z)
int z;

{
   if(z+Start >= NameCount) return;
   if(CurItem != -1) InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   CurItem = z;
   InvertRect(91,71+CurItem*8,215,78+CurItem*8);
   }
   
   

DoItemScroll()

{
   int x;
   
   x = Start;
   if(MousePos.y <= 91) {
      FlashButton(216,70,228,91);
	  Start -= 5;
	  }
   else {
      FlashButton(216,91,228,111);
      if((Start+5) >= NameCount) return;
	  Start += 5;
      }
   if(Start < 0) Start = 0;
   if(Start != x) DrawItemNames();
   }
   
   

DrawItemBox()

{
   SetMarList();
   SaveBuffer();
   SetColor(6);
   RectangleFill(85,65,235,135);
   SetColor(1);
   Rectangle(85L,65L,235L,135L);
   SetColor(15);
   RectangleFill(90,70,216,111);
   SetColor(11);
   Rectangle(90L,70L,216L,111L);
   SetColor(15);
   RectangleFill(216,70,228,111);
   SetColor(11);
   Rectangle(216L,70L,228L,111L);
   Rectangle(216L,91L,228L,111L);
   SetColor(10);
   MoveTo(222,76);
   DrawTo(222,87);
   MoveTo(219,79);
   DrawTo(222,76);
   DrawTo(225,79);
   MoveTo(222,95);
   DrawTo(222,107);
   DrawTo(225,103);
   MoveTo(222,107);
   DrawTo(219,103);
   SetColor(15);
   RectangleFill(105,119,155,130);
   }
   
   
   
DoDropObject()

{
   int x,xu,done = 0;
   
   if(obs[Level][MapOrigin.x+4][MapOrigin.y+4] != 0) {
      IconMessage(6);
      return;
      }
   DrawItemBox();
   SetColor(12);
   Rectangle(105L,119L,155L,130L);
   PrintLineXY("DROP",118,122);
   SetColor(15);
   RectangleFill(165,119,215,130);
   SetColor(12);
   Rectangle(165L,119L,215L,130L);
   PrintLineXY("CANCEL",172,122);
   Start = 0;
   DrawItemNames();
   do {
      x = mygetch();
	  if(x > 0) xu = toupper(x);
	  if(x == 2000) SelectUp();
	  else if(x == 2002) SelectDown();
	  else if(x == 3000) {
	     MousePos.y = 0;
		 DoItemScroll();
		 }
	  else if(x == 3001) {
	     MousePos.y = 199;
		 DoItemScroll();
		 }
	  else if(xu == 'C') done = 2;
	  else if(xu == 'D') done = 1;
	  else if(x == -1) {
	     if(PtInRect(&MousePos,91,71,215,110)) 
		  SelectItem((MousePos.y - 71) / 8);
	     if(PtInRect(&MousePos,105,119,155,130)) done = 1;
	     if(PtInRect(&MousePos,165,119,215,130)) done = 2;
         if(PtInRect(&MousePos,216,71,228,111)) DoItemScroll();
		 }
	  } while(done == 0);
   FlashButton(done * 60 + 45,119,done * 60 + 95,130);
   LoadBuffer();
   if((CurItem != -1) && (done == 1)) {
      CurItem += Start;
	  DropIt();
	  }
   }



ShowTText(z)
int z;

{
   int x;
   
   SaveBuffer();
   SetColor(15);
   RectangleFill(30,160,290,195);
   SetColor(12);
   Rectangle(30L,160L,290L,195L);
   x = 160 - strlen(Quest->ttext[z].itext) * 3;
   PrintLineXY(Quest->ttext[z].itext,x,165);
   Rectangle(135L,179L,185L,190L);
   PrintLineXY("OK",154,182);
   KeyWait();
   FlashButton(135,180,185,189);
   LoadBuffer();
   Quest->ttext[z].read = TRUE;
   }



ShowWriting(z)
int z;

{
   int x;
   
   if(strcmp(Quest->object[z].itext," ") == 0) return;
   x = 160 - strlen(Quest->object[z].itext) * 3;
   SaveBuffer();
   SetColor(15);
   RectangleFill(30,50,290,95);
   SetColor(12);
   Rectangle(30L,50L,290L,95L);
   if(Quest->object[z].type != 11) PrintLineXY("Writing on Object:",106,55);
   else PrintLineXY("The Prisoner Says:",106,55);
   PrintLineXY(Quest->object[z].itext,x,65);
   Rectangle(135L,79L,185L,90L);
   PrintLineXY("OK",154,82);
   KeyWait();
   FlashButton(135,80,185,89);
   LoadBuffer();
   }



DoTakeObject(x,y,mflag)
int x,y;
Boolean mflag;

{
   int Elm,z,i,c;
   
   if((x <= 5) && (x >= 3) && (y <= 5) && (y >= 3)) {
      if(!mflag) GetKeyCoord(&x,&y);
      x += MapOrigin.x;
      y += MapOrigin.y;
      Elm = obs[Level][x][y];
      if((Elm >= 9) && (Elm <= 100)) {
         z = 0;
         while((z < 15) && (Quest->marine[CurMarine].inventory[z] != 0)) z++;
         if (z < 15) {
            i = FindObject(x,y);
            if((Quest->marine[CurMarine].encumbrance +
             objenc[(Elm-9)/2]) > Quest->marine[CurMarine].maxenc) 
             IconMessage(82);
            else {
               CurMove--;
               Quest->marine[CurMarine].vitality--;
               Quest->marine[CurMarine].inventory[z] = i+1;
               Quest->marine[CurMarine].encumbrance += objenc[(Elm-9)/2];
               obs[Level][x][y] = 0;
               Quest->object[i].held = TRUE;
			   if(Quest->object[i].type == 51) Quest->object[i].type = 49;
               ShowWriting(i);
			   SetInvis(CurMarine);
               DrawMarineWindow();
               }
            }
         else IconMessage(1);
         }
      else IconMessage(5);
      }
   }



DoShaft()

{
   int x,y,z,NewLevel;
   
   z = Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4];
   x = Quest->marine[CurMarine].x;
   y = Quest->marine[CurMarine].y;
   if((z == 34) || (z == 35)) {
      if(z == 34) NewLevel = Level + 1;
      else NewLevel = Level - 1;
      if(CurMove < 5) IconMessage(8);
      else if((GetMarL(NewLevel,x,y) != 15) ||
       ((!CheckMove(NewLevel,x,y)) && (obs[NewLevel][x][y] < 100)))
       IconMessage(35);
      else {
         SoundEff(13);
         CurMove -= 5;
         Quest->marine[CurMarine].vitality--;
         SetMarL(Level,x,y,15);
         Level = NewLevel;
         if(FindEnemy(x,y) != numopp) KillNEEnemy(FindEnemy(x,y));
         SetMarL(Level,x,y,CurMarine);
         Quest->marine[CurMarine].level = Level;
         DrawMapWindow();
         UpdateMarine();
         }
      }
   else IconMessage(7);
   }



DoShoot(tx,ty)
int tx,ty;

{

   Point p1,p2;
   int x,y,acc,el,amuse = 1,weap,x1,y1;
   
   if(Quest->marine[CurMarine].gun == 2) amuse = 2;
   tx += MapOrigin.x;
   ty += MapOrigin.y;
   el = Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4];
   /* Identify the target coordinate */
   switch(Quest->marine[CurMarine].facing) {
      case 0: SetPt(&p1,111,92);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 1: SetPt(&p1,113,91);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 2: SetPt(&p1,113,94);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 3: SetPt(&p1,111,98);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 4: SetPt(&p1,106,98);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 5: SetPt(&p1,104,97);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 6: SetPt(&p1,102,94);
              FindTarget(tx,ty,4,&x,&y);
              break;
      case 7: SetPt(&p1,104,91);
              FindTarget(tx,ty,4,&x,&y);
              break;
      }
   SetPt(&p2,(x-MapOrigin.x)*24+12,(y-MapOrigin.y)*18+25);
   if(CurMove < 3) IconMessage(9);
   else if((Heater[CurMarine] > 4) && (amuse == 2)) IconMessage(73);
   else if(Quest->marine[CurMarine].ammo < amuse) IconMessage(81);
   else {
	  weap = Quest->marine[CurMarine].gun;
      if(weap < 2) SoundEff(3);
	  else SoundEff(4);
      CurMove -= 3;
      Quest->marine[CurMarine].vitality -= 3;
      Quest->marine[CurMarine].ammo -= amuse;
	  if(amuse == 2) Heater[CurMarine]++;
      SetColor(5);
      MoveTo(p1.x,p1.y);
      DrawTo(p2.x,p2.y);
	  acc = Quest->marine[CurMarine].shoot;
	  if(el == 83) acc += 5;
      if(Rnd(100) <= acc) {
         if((Quest->map[Level][x][y] >= 64)
          && (Quest->map[Level][x][y] <= 66) && (weap > 0)) SoundEff(9);
		 if(weap == 0) {
            SoundEff(2);
            x1 = x - MapOrigin.x;
            y1 = y - MapOrigin.y;
            BlitElement(99,x1,y1,0,notSrcBic);
            BlitElement(98,x1,y1,0,srcOr);
		    }
         Damage(Quest->marine[CurMarine].gun,Level,x,y);
         if(CurMarine == 9) Quest->slshots++;
         }
      DrawMapWindow();
      UpdateMarine();
      }
   }



GetKeyCoord(x,y)
int *x,*y;

{
   int c;
   
   BlitElement(116,4,4,0,notSrcBic);
   BlitElement(7,4,4,0,srcOr);
   *x = 4;
   *y = 4;
   do {
      c = mygetch();
	  if(c == -1) {
	     ConvertMouseXY(x,y);
		 return;
		 }
      } while((c < 2000) || (c > 2008));
   if(c == 2000) (*y)--;
   if(c == 2001) (*x)++;
   if(c == 2002) (*y)++;
   if(c == 2003) (*x)--;
   if(c == 2004) { (*x)++; (*y)--; }
   if(c == 2005) { (*x)++; (*y)++; }
   if(c == 2006) { (*x)--; (*y)++; }
   if(c == 2007) { (*x)--; (*y)--; }
   }
   
   
   
OpenIt(x,y)
int x,y;

{
   int Elm,Elm2,z;
   
   Elm = Quest->map[Level][x][y];
   Elm = ((Elm-36) % 4) + 48;
   if(Elm == 51) Elm = 52;
   Quest->map[Level][x][y] = Elm;
   if((Elm < 50) && (y > 0)) {
      Elm2 = Quest->map[Level][x][y-1];
	  z = (Elm2-36) % 4;
  	  if((Elm2 >= 36) && (Elm2 <= 47) && (z >= 2))
	   Elm2 = (z-2)*2 + 50;
      Quest->map[Level][x][y-1] = Elm2;
      }
   else if((Elm >= 50) && (y < 49)) {
      Elm2 = Quest->map[Level][x][y+1];
      z = (Elm2-36) % 4;
	  if((Elm2 >= 36) && (Elm2 <= 47) && (z < 2))
	  Elm2 = z + 48;
      Quest->map[Level][x][y+1] = Elm2;
      }
   }



DoOpenDoor(x,y,mflag)
int x,y;
Boolean mflag;

{
   int Elm,Elm2,z;
   
   if((x <= 5) && (x >= 3) && (y <= 5) && (y >= 3)) {
      if(!mflag) GetKeyCoord(&x,&y);
	  ShapeCursor(0);
      x += MapOrigin.x;
      y += MapOrigin.y;
      Elm = Quest->map[Level][x][y];
      if((Elm >= 36) && (Elm <= 47)) {
	     if(Elm < 40) z = 10;
		 else if(Elm > 43) z = 3;
		 else z = 5;
         if(CurMove < z) {
		    if(z == 3) IconMessage(45);
		    else if(z == 5) IconMessage(46);
			else IconMessage(47);
            }
         else {
            SoundEff(12);
		    OpenIt(x,y);
            CurMove -= z;
            Quest->marine[CurMarine].vitality -= z;
            DrawMarineWindow();
            }
         }
      else IconMessage(3);
      }
   }



DoLook(x,y,mflag)
int x,y;
Boolean mflag;

{
   int Elm,x1,y1,z;
   char astr[80],bstr[30];
   
   if((x <= 8) && (x >= 0) && (y <= 8) && (y >= 0)) {
      SetSee(3);
	  x1 = x;
	  y1 = y;
      x += MapOrigin.x;
      y += MapOrigin.y;
      Elm = Quest->map[Level][x][y];
      SaveBuffer();
      SetColor(15);
      RectangleFill(5,160,314,195);
      SetColor(12);
      Rectangle(5L,160L,314L,195L);
	  if((x < 0) || (y < 0) || (x > 49) || (y > 49))
	   PrintLineXY("That is outside the combat area.",55,165);
	  else if(!see[x1+4][y1+4]) PrintLineXY("You cannot see what is there.",73,165);
	  else {
	     strcpy(astr,namer[ElmName[Elm-18]]);
		 strcat(astr," (");
		 itoa(movc[Elm],bstr,10);
		 strcat(astr,bstr);
		 strcat(astr,"MP)");
		 z = obs[Level][x][y];
		 if((z < 100) && (z > 0)) {
		    strcat(astr,", ");
			strcat(astr,namer[ElmName[z+82]]);
			}
	     else if(z >= 100) {
		    strcat(astr,", ");
			strcat(astr,namer[z-43]);
			}
		 z = GetMarL(Level,x,y);
		 if(z != 15) {
		    strcat(astr,", ");
			strptoc(bstr,Quest->marine[z].name);
			strcat(astr,bstr);
			}
		 astr[51] = 0;
		 z = (310-(strlen(astr) * 6)) / 2 + 5;
		 PrintLineXY(astr,z,165);
		 }
      Rectangle(135L,179L,185L,190L);
      PrintLineXY("OK",154,182);
      KeyWait();
      FlashButton(135,180,185,189);
      LoadBuffer();
      }
   }



DoPsiCast(x,y)
int x,y;

{
   int c,x1,y1,z;
   
   x += MapOrigin.x;
   y += MapOrigin.y;
   if(Quest->marine[CurMarine].psi) {
      if(CurMove >= 10) {
         if(Rnd(100) <= Quest->marine[CurMarine].vitality) {
            /* Do Psi effect */
            SoundEff(2);
            x1 = x - MapOrigin.x;
            y1 = y - MapOrigin.y;
            BlitElement(99,x1,y1,0,notSrcBic);
            BlitElement(98,x1,y1,0,srcOr);
            /* Look for an enemy marine */
            c = FindEnemy(x,y);
            if(c != numopp) {
			   z = Quest->opponent[c].type;
			   if(z <= 4) Quest->opponent[c].vitality = 0;
			   }
            c = FindMarine(x,y);
            if(c != 10) Quest->marine[c].vitality = 0;
            Delay(25L);
            CurMove -= 10;
            Quest->marine[CurMarine].vitality -= 10;
            DrawMapWindow();
            DrawMarineWindow();
            }
         else {
            IconMessage(33);
			CurMove -= 10;
            Quest->marine[CurMarine].vitality -= 10;
            DrawMarineWindow();
            }
         }
      else IconMessage(32);
      }
   else IconMessage(34);
   }



DoIcons()

{
   int x,y;
   
   x = MousePos.x / 24;
   y = (MousePos.y - 16) / 18;
   ShapeCursor(0);
   switch(IconFlag) {
      case 3: DoTakeObject(x,y,TRUE);
              break;
      case 5: DoOpenDoor(x,y,TRUE);
              break;
      case 8: DoLook(x,y,TRUE);
              break;
      }
   DrawMapWindow();
   }
   


ResetIcons()

{
   ShapeCursor(0);
   InvertIcon(IconFlag);
   IconFlag = 0;
   PointFlag = 0;
   MoveCursor(0);
   }


InvertIcon(id)
int id;

{
   if(id == 1) InvertRect(DX1+2,DY1+2,DX2-2,DY2-2); 
   else if(id == 2) InvertRect(SX1+2,SY1+2,SX2-2,SY2-2); 
   else if(id == 3) InvertRect(TX1+2,TY1+2,TX2-2,TY2-2); 
   else if(id == 4) InvertRect(UX1+2,UY1+2,UX2-2,UY2-2); 
   else if(id == 5) InvertRect(KX1+2,KY1+2,KX2-2,KY2-2); 
   else if(id == 6) InvertRect(AX1+2,AY1+2,AX2-2,AY2-2); 
   else if(id == 8) InvertRect(IX1+2,IY1+2,IX2-2,IY2-2); 
   }
   
   
   
FlashIcon(id)
int id;

{
   if(id == 1) FlashButton(DX1+2,DY1+2,DX2-2,DY2-2); 
   else if(id == 2) FlashButton(SX1+2,SY1+2,SX2-2,SY2-2); 
   else if(id == 3) FlashButton(TX1+2,TY1+2,TX2-2,TY2-2); 
   else if(id == 4) FlashButton(UX1+2,UY1+2,UX2-2,UY2-2); 
   else if(id == 5) FlashButton(KX1+2,KY1+2,KX2-2,KY2-2); 
   else if(id == 6) FlashButton(AX1+2,AY1+2,AX2-2,AY2-2); 
   else if(id == 8) FlashButton(IX1+2,IY1+2,IX2-2,IY2-2); 
   }
   
   
   
CheckMark(x,y,b)
int x,y;
Boolean b;

{
   if(b) {
      SetColor(12);
      Rectangle((long)x,(long)y,(long)(x+12),(long)(y+9));
	  }
   if(b) SetColor(9);
   else SetColor(15);
   MoveTo(x+3,y+5);
   DrawTo(x+5,y+8);
   DrawTo(x+11,y-2);
   if(!b) {
      SetColor(12);
      Rectangle((long)x,(long)y,(long)(x+12),(long)(y+9));
	  }
   }
   
   

ArmWeapon()

{
   int x,y;
   int done = 0,c,SwapCond,armer;
   Boolean avail[5];
   
   for(y = 1; y < 5; y++) avail[y] = FALSE;
   avail[0] = TRUE;
   if(Quest->marine[CurMarine].psi) avail[4] = TRUE;
   for(y = 0; y < 15; y++) {
      x = GetType(CurMarine,y);
      if(x == 25) avail[2] = TRUE;
      if(x == 27) avail[3] = TRUE;
      if(x == 29) avail[1] = TRUE;
	  }
   armer = Quest->marine[CurMarine].gun + 1;
   SaveBuffer();
   SetColor(15);
   RectangleFill(80,40,245,180);
   SetColor(6);
   Rectangle(80L,40L,245L,180L);
   PrintLineXY("Ready Weapon",124,45);
   PrintLineXY("Unarmed",130,60);
   CheckMark(100,58,FALSE);
   PrintLineXY("Stun Pistol",130,80);
   if(!avail[1]) PrintLineXY("Unavailable",140,88);
   CheckMark(100,78,FALSE);
   PrintLineXY("Laser Pistol",130,100);
   if(!avail[2]) PrintLineXY("Unavailable",140,108);
   CheckMark(100,98,FALSE);
   PrintLineXY("Rifle",130,120);
   if(!avail[3]) PrintLineXY("Unavailable",140,128);
   CheckMark(100,118,FALSE);
   PrintLineXY("Psionic Stun",130,140);
   if(!avail[4]) PrintLineXY("Unavailable",140,148);
   CheckMark(100,138,FALSE);
   CheckMark(100,armer*20+58,TRUE);
   Rectangle(87L,164L,137L,175L);
   PrintLineXY("OK",106,167);
   Rectangle(187L,164L,237L,175L);
   PrintLineXY("CANCEL",194,167);
   c = 0;
   do {
      SwapCond = -1;
	  c = mygetch();
	  if(c == -1) {
	     if(PtInRect(&MousePos,87,164,137,175)) done = 1;
	     if(PtInRect(&MousePos,187,164,237,175)) done = 2;
		 else if(PtInCheck(&MousePos,100,58)) SwapCond = 0;
	     else if(PtInCheck(&MousePos,100,78)) SwapCond = 1;
	     else if(PtInCheck(&MousePos,100,98)) SwapCond = 2;
	     else if(PtInCheck(&MousePos,100,118)) SwapCond = 3;
	     else if(PtInCheck(&MousePos,100,138)) SwapCond = 4;
		 }
	  else if(c > 31) {
		 c = toupper(c);
		 if(c == 'O') done = 1;
		 if(c == 'C') done = 2;
		 if(c == 'U') SwapCond = 0;
		 if(c == 'S') SwapCond = 1;
		 if(c == 'L') SwapCond = 2;
		 if(c == 'R') SwapCond = 3;
		 if(c == 'P') SwapCond = 4;
		 }
	  if((SwapCond != -1) && avail[SwapCond]) {
		 CheckMark(100,armer*20+58,FALSE);
	     armer = SwapCond;
		 CheckMark(100,SwapCond*20+58,TRUE);
		 }
	  } while(done == 0);
   FlashButton(87+(done-1)*100,164,137+(done-1)*100,175);
   LoadBuffer();
   if(done == 1) Quest->marine[CurMarine].gun = armer - 1;
   }



SetIcons(id,mflag)
int id;
Boolean mflag;

{
   if(IconFlag != 0) return;
   if(BeltFlag) {
      IconFlag = id + 1;
       if(IconFlag == 4) {
           BeltOff();
           DrawMarineWindow();
           }
       IconFlag = 0;
       DrawMapWindow();
       }
   else {
      IconFlag = id + 1;
      FlashIcon(IconFlag);
      if((id == 2) || (id == 4) || (id == 7)) InvertIcon(IconFlag);
      switch(IconFlag) {
         case 1: DoDropObject();
                 IconFlag = 0;
                 break;
         case 2: DoShaft();
                 IconFlag = 0;
                 break;
         case 3: ShapeCursor(5);
		         if(!mflag) {
				    DoTakeObject(4,4,FALSE);
					IconFlag = 0;
					ShapeCursor(0);
					DrawMapWindow();
					InvertIcon(3);
					}
                 break;
         case 4: DoUseObject();
                 IconFlag = 0;
                 break;
         case 5: ShapeCursor(3);
		         if(!mflag) {
				    DoOpenDoor(4,4,FALSE);
					IconFlag = 0;
					ShapeCursor(0);
					DrawMapWindow();
					InvertIcon(5);
					}
                 break;
         case 6: if(modifier) DetonateDemo();
		         else ArmWeapon();
                 IconFlag = 0;
                 break;
         case 8: break;
         }
      if((IconFlag != 0) || (PointFlag != 0)) SkipIt = TRUE;
      }
   }
   


BeltOff()

/* Turn off the GravBelt */

{
   int x,y,i;
   
   x = MapOrigin.x+4;
   y = MapOrigin.y+4;
   i = Quest->map[Level][x][y];
   if((i == 73) || (i == 70)) KillMarine(CurMarine);
   if(Rnd(100) <= 5) {
      /* Belt explodes */
      IconMessage(28);
      /* Damage the marine */
      Damage(3,Level,x,y);
      }
   BeltFlag = FALSE;
   }





