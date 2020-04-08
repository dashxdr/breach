/* breach26.c - Enemy Movement */

#include   "breach2.h"

/* External Procedures */
extern char *malloc();

/* External Variables */
extern FILE *ser;
extern Byte see[17][17];
extern Byte passer[];
extern struct QstStru *Quest;
extern Byte obs[5][50][50];
extern Byte mar[5][50][50];
extern Byte movc[45];
extern Boolean Changed;
extern Point MapOrigin;
extern int Level;
extern int CurMarine,CurMove;
extern Boolean outrng;
extern struct nodestru *finnode,*tree;
extern struct pathstru path[2][169];
extern int CurEnemy,curpath,curx,cury,destx,desty,targx,targy;
extern Byte fill[13][13];
extern int pquan[2];
extern int tnum,trng,tdir;
extern Boolean arrived,Invis[],InvisView[],lifted;



FindNSTarget(tx,ty,range,x,y)
int tx,ty,range;
int *x,*y;

{
   FindTarget(tx,ty,range,x,y);
   return;
   }
      

      
SetupPathMap()

{
   int i,j,n,m,l,z,etype;
   
   etype = Quest->opponent[CurEnemy].type;
   finnode = NULL;
   tree = NULL;
   l = Level;
   for(i = 0; i <= 12; i++) for(j = 0; j <= 12; j++) {
      n = (curx - 6) + i;
      m = (cury - 6) + j;
      if((n >= 0) && (n <= 49) && (m >= 0) && (m <= 49)) {
         z = Quest->map[l][n][m];
         if(passer[z] == 1) fill[i][j] = 0;
		 else if((passer[z] == 2) && (etype == 4)) fill[i][j] = 0; 
         else fill[i][j] = 1;
		 if(z < 34) fill[i][j] = 1;
		 if(Quest->opponent[CurEnemy].type == 10) fill[i][j] = 0;
		 if((z == 19) || (z == 21) || (z == 27) || (z == 29)) fill[i][j] =
		  0;
		 if(GetMarH(l,n,m) != 0) fill[i][j] = 1;
         if(obs[l][n][m] == 9) fill[i][j] = 1;
         if(obs[l][n][m] == 11) fill[i][j] = 1;
		 if(obs[l][n][m] >= 100) fill[i][j] = 1;
		 if(z == 91) fill[i][j] = 1;
         }
      else fill[i][j] = 1;
      }
   if(GetMarH(l,curx,cury) != 0) fill[6][6] = 1;
   else fill[6][6] = 0;
   z = Quest->map[l][curx][cury];
   if((z == 26) || (z == 18)) {
      fill[6][7] = 1;
	  fill[7][6] = 1;
      }
   else if((z == 28) || (z == 20)) {
      fill[5][6] = 1;
	  fill[6][7] = 1;
      }
   }

   
   
struct nodestru *AddNode(x,y,thisnode,prev)
int x,y,prev;
struct nodestru *thisnode;

{
   struct nodestru *newnode;
   long templ;
   
   if(thisnode == NULL) {
      tree = (struct nodestru *)malloc(24);
      tree->x = x;
      tree->y = y;
      tree->uplink = NULL;
      tree->l1 = NULL;
      tree->l2 = NULL;
      tree->l3 = NULL;
      tree->l4 = NULL;
      return(tree);
      }
   else {
      newnode = (struct nodestru *)malloc(24);
      newnode->x = x;
      newnode->y = y;
      newnode->uplink = (struct nodestru *)thisnode;
      newnode->l1 = (struct nodestru *)NULL;
      newnode->l2 = (struct nodestru *)NULL;
      newnode->l3 = (struct nodestru *)NULL;
      newnode->l4 = (struct nodestru *)NULL;
      switch(prev) {
         case 1: thisnode->l3 = (struct nodestru *)newnode;
                 break;
         case 2: thisnode->l4 = (struct nodestru *)newnode;
                 break;
         case 3: thisnode->l1 = (struct nodestru *)newnode;
                 break;
         case 4: thisnode->l2 = (struct nodestru *)newnode;
                 break;
         }
      if((x == destx) && (y == desty)) finnode = newnode;
      return(newnode);
      }
   }
   
   
   
FindNode(x,y,prev,nownode)
int x,y,prev;
struct nodestru *nownode;

{
   if((x >= 0) && (x <= 12) && (y >= 0) && (y <= 12) && (finnode == NULL))
    if(fill[x][y] == 0) {
      nownode = AddNode(x,y,nownode,prev);
      fill[x][y] = 1;
      if(curpath == 0) {
         if(prev != 4) FindNode(x-1,y,2,nownode);
         if(prev != 3) FindNode(x,y+1,1,nownode);
         if(prev != 2) FindNode(x+1,y,4,nownode);
         if(prev != 1) FindNode(x,y-1,3,nownode);
         }
      else {
         if(prev != 1) FindNode(x,y-1,3,nownode);
         if(prev != 2) FindNode(x+1,y,4,nownode);
         if(prev != 3) FindNode(x,y+1,1,nownode);
         if(prev != 4) FindNode(x-1,y,2,nownode);
         }
      }
   }
   
   

BuildTree(x,y)
int x,y;

{
   FindNode(x,y,0,tree);
   }
   
   

ShowPNode(nptr)
struct nodestru *nptr;

{
   long t;
   
   path[curpath][pquan[curpath]].x = nptr->x;
   path[curpath][pquan[curpath]].y = nptr->y;
   pquan[curpath]++;
   if(nptr->uplink != NULL) ShowPNode(nptr->uplink);
   }
   
   

SetPath()

{
   ShowPNode(finnode);
   }
   
   

Boolean Adj(p1,p2)
int p1,p2;

{
   int dx,dy;
   
   dx = abs(path[curpath][p1].x - path[curpath][p2].x);
   dy = abs(path[curpath][p1].y - path[curpath][p2].y);
   if(((dx == 0) && (dy == 1)) || ((dx == 1) && (dy == 0))) return(TRUE);
   return(FALSE);
   }
   
   

Compress(p1,p2)
int p1,p2;

{
   int del,x;
   
   del = p2 - p1 - 1;
   for(x = p2; x <= (pquan[curpath] - 1); x++)
    path[curpath][x-del] = path[curpath][x];
   pquan[curpath] -= del;
   }
   
   

OptimizePath()

{
   int ptr1,ptr2;
   
   if(pquan[curpath] > 3) {
      ptr1 = 0;
      do {
         ptr2 = ptr1 + 2;
         do {
            if(Adj(ptr1,ptr2)) Compress(ptr1,ptr2);
            ptr2++;
            } while(ptr2 < (pquan[curpath] - 1));
         ptr1++;
         } while(ptr1 < (pquan[curpath] - 2));
      }
   }
   
   

DestroyNode(nptr)
struct nodestru *nptr;

{
   if(nptr->l1 != NULL) DestroyNode(nptr->l1);
   if(nptr->l2 != NULL) DestroyNode(nptr->l2);
   if(nptr->l3 != NULL) DestroyNode(nptr->l3);
   if(nptr->l4 != NULL) DestroyNode(nptr->l4);
   free(nptr);
   }
   
   
ConvertPath()

/* Convert path data from local to map coordinates */

{
   int i;
   
   for(i = 0; i <= (pquan[curpath] - 1); i++) {
      path[curpath][i].x += (curx - 6);
      path[curpath][i].y += (cury - 6);
      }
   }
   
   

FindPath()

/* Find the shortest path to the destination */

{
   int i;
   
   for(i = 0; i <= 1; i++) {
      pquan[i] = 0;
      curpath = i;
      SetupPathMap();
      if(fill[6][6] == 1) {
	     finnode = NULL;
		 return;
		 }
      BuildTree(6,6);
      if(finnode != NULL) {
         SetPath();
         OptimizePath();
         }
      DestroyNode(tree);
      }
   if(pquan[0] < pquan[1]) curpath = 0;
   else curpath = 1;
   ConvertPath();
   }
   
   
   
EUseShaft(dir)
int dir;

{
   int x;
   
   if((CurMove >= 5) && 
    (obs[Level+dir][curx][cury] < 100) &&
    (obs[Level+dir][curx][cury] != 9) &&
    (obs[Level+dir][curx][cury] != 11)) {
	  x = GetMarL(Level+dir,curx,cury);
	  obs[Level][curx][cury] = 0;
      Level += dir;
      Quest->opponent[CurEnemy].level = Level;
      obs[Level][curx][cury] = Quest->opponent[CurEnemy].type+100;
	  if(x != 15) {
	     DrawMapWindow();
	     KillMarine(x);
		 DrawMapWindow();
		 }
      lifted = TRUE;
	  CurMove -= 5;
      }
   else CurMove = 0;
   }



MoveOneSquare()

/* Move the current enemy one square */

{
   int n,m,i,j,z,el,oldo,typ;
   
   if(pquan[curpath] >= 2) {
      n = path[curpath][pquan[curpath] - 2].x;
      m = path[curpath][pquan[curpath] - 2].y;
      z = obs[Level][n][m];
      if((FindMarine(n,m) == 10) && (FindEnemy(n,m) == numopp)
       && (z != 9) && (z != 11)) {
	     oldo = FindObject(n,m);
         if((oldo != numobj) && (Quest->object[oldo].type != 39)) {
            Quest->object[oldo].type = 0;
            obs[Level][n][m] = 0;
			oldo = numobj;
            }
         obs[Level][curx][cury] = 0;
         if(n < curx) {
		    if(m < cury) Quest->opponent[CurEnemy].facing = 7;
			else if(m > cury) Quest->opponent[CurEnemy].facing = 5;
			else Quest->opponent[CurEnemy].facing = 6;
			}
		 else if(n > curx) {
		    if(m < cury) Quest->opponent[CurEnemy].facing = 1;
			else if(m > cury) Quest->opponent[CurEnemy].facing = 3;
			else Quest->opponent[CurEnemy].facing = 2;
			}
		 else {
            if(m < cury) Quest->opponent[CurEnemy].facing = 0;
            if(m > cury) Quest->opponent[CurEnemy].facing = 4;
			}
         curx = n;
         cury = m;
		 typ = Quest->opponent[CurEnemy].type;
         Quest->opponent[CurEnemy].x = curx;
         Quest->opponent[CurEnemy].y = cury;
         SetPt(&MapOrigin,curx-4,cury-4);
		 SetSee(4);
         obs[Level][n][m] = Quest->opponent[CurEnemy].type+100;
         pquan[curpath]--;
         if(pquan[curpath] == 2) arrived = TRUE;
		 z = Quest->map[Level][curx][cury];
         CurMove -= movc[z];
         if((z >= 36) && (z <= 47)) OpenIt(curx,cury);
         if(z == 35) EUseShaft(-1);
         else if(z == 34) EUseShaft(1);
		 else if((oldo != numobj) && (z >= 84) && (z <= 86) &&
		  (typ != 4) && (typ != 10)) {
		    ShapeCursor(0);
            IconMessage(76);
			ShapeCursor(1);
		    DetonateProx(curx,cury);
			}
		 if(Quest->opponent[CurEnemy].health <= 0) CurMove = 0;
         }
      else CurMove = 0;
      }
   else arrived = TRUE;
   outrng = FALSE;
   }
   
   

InitCurrentEnemy()

{
   int x,z,p;
   
   curx = Quest->opponent[CurEnemy].x;
   cury = Quest->opponent[CurEnemy].y;
   z = Quest->opponent[CurEnemy].vitality;
   SetPt(&MapOrigin,curx-4,cury-4);
   SetSee(4);
   Level = Quest->opponent[CurEnemy].level;
   /* Set moves */
   if(!lifted) {
      if((z == 0) || (z > 100)) CurMove = 0;
      else {
         CurMove = Quest->opponent[CurEnemy].vitality * 2;
         if(CurMove > 120) CurMove = 120;
         CurMove *= (Quest->opponent[CurEnemy].basemove + 3 +
          Quest->opponent[CurEnemy].bonusmove) -
          ((Quest->opponent[CurEnemy].encumbrance - 2) / 2);
         CurMove /= 100;
         }
	  if(z > 100) Quest->opponent[CurEnemy].vitality--;
	  else {
         /* Set vitality */
         Quest->opponent[CurEnemy].vitality +=
          Quest->opponent[CurEnemy].health / 4;
         if(Quest->opponent[CurEnemy].vitality > 100)
          Quest->opponent[CurEnemy].vitality = 100;
		 }
	  }
   /* Set flags */
   finnode = NULL;
   arrived = FALSE;
   outrng = FALSE;
   lifted = FALSE;
   for(x = 0; x < 10; x++) {
      if(Invis[x]) {
	     z = Quest->map[Quest->marine[x].level][Quest->marine[x].x]
		  [Quest->marine[x].y];
		 p = 30;
		 if(z == 88) p = 20;
		 if(z == 90) p = 10;
		 if(z == 89) p = 15;
	     if(Rnd(100) < p) InvisView[x] = FALSE;
		 else InvisView[x] = TRUE;
		 }
      else InvisView[x] = FALSE;
	  }
   }
   
   

MarineCrossTarget(trg,rng,dir)
int *trg,*rng,*dir;

{
   int x,y,prio,tmp,trng,i,tdir,tx,ty;
   
   prio = 0;
   *trg = -1;
   for(i = 0; i < 10; i++)
    if((Quest->marine[i].class != 0) && (Quest->marine[i].condition > 0)
    && (Quest->marine[i].level == Level) && (!InvisView[i])) {
      tdir = -1;
      x = curx - Quest->marine[i].x;
      y = cury - Quest->marine[i].y;
	  if(abs(x) == abs(y)) {
	     if(x > 0) {
		    if(y > 0) tdir = 7;
			else tdir = 5;
			}
	     else  {
		    if(y > 0) tdir = 1;
			else tdir = 3;
			}
		 trng = abs(x);
	     }
      if((abs(x) <= 4) && (y == 0)){
         if(x > 0) tdir = 6;
         else tdir = 2;
         trng = abs(x);
         }
      if((abs(y) <= 4) && (x == 0)){
         if(y > 0) tdir = 0;
         else tdir = 4;
         trng = abs(y);
         }
      tmp = Quest->marine[i].shoot + Quest->marine[i].crack +
       Quest->marine[i].detect + (Quest->marine[i].encumbrance * 20);
      FindNSTarget(Quest->marine[i].x,Quest->marine[i].y,4,&tx,&ty);
      if((tmp > prio) && (tdir != -1) && (FindMarine(tx,ty) == i)
	   && (see[tx-MapOrigin.x+4][ty-MapOrigin.y+4])) {
         prio = tmp;
         *dir = tdir;
         *rng = trng;
         *trg = i;
         targx = tx;
         targy = ty;
         Quest->opponent[CurEnemy].facing = *dir;
         }
      }
   }
   
   

int MarineAreaTarget()

/* Find a target marine in the area */

{
   int x,y,prio,target,i,tmp;
   
   prio = 0;
   target = -1;
   for(i = 0; i < 10; i++)
    if((Quest->marine[i].class != 0) && (Quest->marine[i].condition > 0)
    && (Quest->marine[i].level == Level) && (!InvisView[i]) &&
    (Quest->marine[i].x != -1)) {
      x = curx - Quest->marine[i].x;
      y = cury - Quest->marine[i].y;
     
    if(GetMarL(Quest->marine[i].level,Quest->marine[i].x,Quest->marine[i].y)
       != 15)
       if((abs(x) <= 6) && (abs(y) <= 6)) {
         tmp = Quest->marine[i].shoot + Quest->marine[i].crack +
          Quest->marine[i].detect + (Quest->marine[i].encumbrance * 20);
         if((tmp > prio) && ((Rnd(100) < 80) || (prio == 0))) {
            prio = tmp;
            target = i;
            }
         }
      }
   return(target);
   }
   
   

Boolean ObjectTargetArea()

/* Find a target object in the area */

{
   int i,j,n,m,high = 0;
   Boolean shafter = FALSE;
   
   if((Quest->opponent[CurEnemy].type == 2) ||
    (Quest->opponent[CurEnemy].type == 3) ||
    (Quest->opponent[CurEnemy].type == 8)) shafter = TRUE;
   for(i = curx-6; i <= curx+6; i++) for(j = cury-6; j <= cury+6; j++)
    if((i >= 0) && (i <= 49) && (j >= 0) && (j <= 49)) {
      n = i - (curx - 6);
      m = j - (cury - 6);
      if(Quest->VCond[2] && (Quest->map[Level][i][j] == 92) &&
       (high < 5)) {
         high = 5;
         destx = n;
         desty = m;
         }
      if(Quest->VCond[0] && (obs[Level][i][j] == 9) &&
       (high < 4)) {
         high = 4;
         destx = n;
         desty = m;
         }
      if(Quest->VCond[1] && (obs[Level][i][j] == 11) &&
       (high < 3)) {
         high = 3;
         destx = n;
         desty = m;
         }
      if(((Quest->map[Level][i][j] == 34) || (Quest->map[Level][i][j]
       == 35)) && (high < 2) && (shafter)) {
         high = 2;
         destx = n;
         desty = m;
         }
      if((Quest->map[Level][i][j] == 86) && (high < 1)) {
         high = 1;
         destx = n;
         desty = m;
         }
      }
   if(high != 0) return(TRUE);
   if(Quest->skill == 0) return(FALSE);
   m = Quest->map[Level][curx][cury];
   if((m == 86) || (m == 83)) return(FALSE);
   destx = (Rnd(7)-3)+6;
   desty = (Rnd(7)-3)+6;
   return(TRUE);
   }
   
   

Boolean MarineForwardTarget()

{
   switch(Quest->opponent[CurEnemy].facing) {
      case 0: FindNSTarget(curx,cury-4,4,&targx,&targy); break;
      case 1: FindNSTarget(curx+4,cury-4,4,&targx,&targy); break;
      case 2: FindNSTarget(curx+4,cury,4,&targx,&targy); break;
      case 3: FindNSTarget(curx+4,cury+4,4,&targx,&targy); break;
      case 4: FindNSTarget(curx,cury+4,4,&targx,&targy); break;
      case 5: FindNSTarget(curx-4,cury+4,4,&targx,&targy); break;
      case 6: FindNSTarget(curx-4,cury,4,&targx,&targy); break;
      case 7: FindNSTarget(curx-4,cury-4,4,&targx,&targy); break;
      }
   if((FindMarine(targx,targy) != 10) &&
    (Quest->opponent[CurEnemy].type != 1) &&
    (Quest->opponent[CurEnemy].type != 6) &&
    (Quest->opponent[CurEnemy].type != 4)) return(TRUE);
   return(FALSE);
   }
   
   

MoveEnemy()

{
   int z;
   
   do {
      MarineCrossTarget(&tnum,&trng,&tdir);
      if(tnum != -1) EnemyAttack();
      } while((CurMove > 0) && (tnum != -1) && (!outrng));
   if(Quest->opponent[CurEnemy].type != 5) {
      tnum = MarineAreaTarget();
      if(tnum != -1) {
         destx = Quest->marine[tnum].x - (curx - 6);
         desty = Quest->marine[tnum].y - (cury - 6);
         FindPath();
         }
      else if(ObjectTargetArea()) {
		 z = Quest->map[Level][destx-6+curx][desty-6+cury];
	     FindPath();
		 if((finnode == NULL) && ((z == 34) || (z == 35))) {
		    if((destx == 6) && (desty == 6)) {
               if(z == 35) EUseShaft(-1);
               else if(z == 34) EUseShaft(1);
			   }
		    }
		 }
      }
   if((finnode != NULL) && (CurMove > 0)) do {
      outrng = FALSE;
      if(MarineForwardTarget()) {
	     EnemyAttack();
		 }
	  else {
         MarineCrossTarget(&tnum,&trng,&tdir);
         if(tnum != -1) EnemyAttack();
         if((tnum == -1) || (outrng)) {
            if(!arrived) MoveOneSquare();
            else EnemyAttack();
		    if(lifted) return;
			}
         }
      } while((CurMove > 0) && (!outrng));
   else do {
      MarineCrossTarget(&tnum,&trng,&tdir);
      if(tnum != -1) EnemyAttack();
      else CurMove = 0;
      } while((CurMove > 0) && (!outrng));
   }



DoEnemies()

/* Do enemy movement and attack */

{
   char a[20];

   ShapeCursor(1);
   ClearSmoke();
   strctop(a,"Enemy Attacks");
   Changed = TRUE;
   TitleMarine(a);
   ClearMstats();
   PrintLineXY("Enemy",230,65);
   PrintLineXY("Movement...",230,75);
   CurEnemy = 0;
   lifted = FALSE;
   do {
      if((Quest->opponent[CurEnemy].type != 0) &&
       (Quest->opponent[CurEnemy].health > 0)) {
         InitCurrentEnemy();
		 MoveEnemy();
         }
      if((!lifted) || (CurMove <= 0)) {
	     CurEnemy++;
		 lifted = FALSE;
		 }
      } while(CurEnemy < numopp);
   CreateMarineWindow();
   ShapeCursor(0);
   }
