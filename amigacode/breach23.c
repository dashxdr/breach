/* breach23.c - Briefing, Objective, Endgame */

#include   "breach2.h"

/* External Procedurtes */
char *itoa();

/* External Variables */
extern int CurMarine,CurMove;
extern unsigned char crushed[sizeof(struct QstStru)];
extern int Movements[10];
extern char *messages[];
extern struct QstStru *Quest;
extern Byte obs[5][50][50];
extern Boolean NewFlag,Changed,Quitting,SoundFlag;
extern int MapWindow;
extern Point MapOrigin,MousePos;
extern int Level;
extern Boolean Flag137,Flag138;
extern char *rankname[];
extern char *rankabbv[];
extern Byte rankenc[];
extern Byte rankbonus[];
extern Byte rankshoot[];
extern Byte rankcrack[];
extern Byte rankdet[];
extern int PointMarine;
extern Byte iconmap[];


CheckRank()

{
   int x;
   char tmp[80];
   
   for(x = 10; x >= 0; x--) 
    if((Quest->marine[9].shoot >= rankshoot[x]) &&
	(Quest->marine[9].crack >= rankcrack[x]) &&
	(Quest->marine[9].detect >= rankdet[x])) break;
   x += 5;
   if(x > Quest->marine[9].class) {
      Quest->marine[9].class = x;
	  Quest->marine[9].maxenc = rankenc[x-5];
	  Quest->marine[9].bonusmove = rankbonus[x-5];
	  strcpy(tmp,rankabbv[x-5]);
	  Quest->marine[9].name[1] = tmp[0];
	  Quest->marine[9].name[2] = tmp[1];
	  strcpy(tmp,"You have been promoted\nto ");
	  strcat(tmp,rankname[x-5]);
	  strcat(tmp,".");
	  messages[55] = tmp;
	  IconMessage(56);
	  PlaySong(2);
	  if(!SoundFlag) KeyWait();
	  LoadBuffer();
	  }
   }
   
   
   
UpgradeLeader()

/* Upgrade the squad leader stats */

{
   IconMessage(48);
   Delay(100L);
   LoadBuffer();
   if(((Quest->slshots >= 5) || (Quest->slkills >= 2)) &&
    (Rnd(100) > Quest->marine[9].shoot)) {
      Quest->marine[9].shoot += Rnd(5) + 1;
      if(Quest->marine[9].shoot > 100) Quest->marine[9].shoot = 100;
      IconMessage(42);
      }
   if((Quest->slcracks > 0) && (Rnd(100) > Quest->marine[9].crack)) {
      Quest->marine[9].crack += Rnd(5) + 1;
      if(Quest->marine[9].crack > 100) Quest->marine[9].crack = 100;
      IconMessage(43);
      }
   if((Quest->sldetects > 0) && (Rnd(100) > Quest->marine[9].detect)) {
      Quest->marine[9].detect += Rnd(5) + 1;
      if(Quest->marine[9].detect > 100) Quest->marine[9].detect = 100;
      IconMessage(44);
      }
   CheckRank();
   }
   
   

DoChain()

{
   FILE *theFile;
   char fname[40],a[253];
   struct MarStru tempm[10];
   int x,y,z = 0,tskill;

   IconMessage(51);
   strcpy(fname,Quest->chain);
   theFile = fopen(fname,"r");
   if(theFile == NULL) {
      IconMessage(50);
      DoDelOld();
      }
   else {
      ShapeCursor(1);
      for(x = 0; x < 10; x++) {
         tempm[x] = Quest->marine[x];
         for(y = 0; y < 15; y++) tempm[x].inventory[y] = 0;
         }
	  tskill = Quest->skill;
      SetLeaderStru();
      strcpy(a,Quest->slname);
      fseek(theFile,21L,0);
      fread(crushed,1,(int)sizeof(*Quest),theFile);
      fclose(theFile);
	  uncrushit();
      SetLInfo(a);
      for(x = 0; x < 9; x++) z += Quest->marine[x].class;
      if(z != 0) z = 9;
      for(x = z; x < 10; x++) {
         Quest->marine[x] = tempm[x];
         Quest->marine[x].encumbrance = 0;
         }
      for(x = 0; x < 10; x++) Quest->marine[x].gun = -1;
	  Quest->skill = tskill;
      DoNGV();
      DoLoader();
      DoNew();
      ShapeCursor(0);
	  Changed = TRUE;
      }
   }



EndGame(code)
int code;

/* Look for the end of the game.  code is as follows:
   0   : Check for end game
   != 0: Vital object # destroyed */
   
{
   Boolean success;
   int i;
   
   if(Quitting) return;
   if((code == 11) && (Quest->VCond[0])) Flag138 = TRUE;
   if((Flag138) && (code == 0)) {
      ShapeCursor(0);
      IconMessage(21);
      DoDelOld();
      return;
      }
   /* Check for destroyed DataPack */
   if((code == 9) && (Quest->VCond[1])) Flag137 = TRUE;
   if((Flag137) && (code == 0)) {
      ShapeCursor(0);
      IconMessage(20);
      DoDelOld();
      return;
      }
   /* Check for time limit */
   if(Quest->TimeLimit == 0) {
      IconMessage(22);
      DoDelOld();
      return;
      }
   /* Check for dead squad leader */
   if(Quest->marine[9].class == 0) {
      ShapeCursor(0);
      IconMessage(24);
      DoDelOld();
      return;
      }
   /* Check for dead squad */
   success = FALSE;
   for(i = 0; i < 10; i++) if(Quest->marine[i].class != 0) success = TRUE;
   if(!success) {
      IconMessage(25);
      DoDelOld();
      return;
      }
   /* Check for completed mission */ 
   success = TRUE;
   if((Quest->VCond[0]) && (!SatObj(11))) success = FALSE;
   if((Quest->VCond[1]) && (!SatObj(9))) success = FALSE;
   if(Quest->VCond[2]) for(i = 0; i < 10; i++)
    if((Quest->marine[i].level != -1) &&
     (Quest->marine[i].class != 0)) success = FALSE;
   if((Quest->VCond[4]) && (!SatDes(9))) success = FALSE;
   if((Quest->VCond[5]) && (!SatOcc())) success = FALSE;
   if((Quest->VCond[3]) && (!SatKill())) success = FALSE;
   if(success) {
      ShowWinScreen();
      UpgradeLeader();
      Quest->victories++;
      if(strcmp(Quest->chain,"") != 0) DoChain();
      else {
         IconMessage(23);
         DoDelOld();
         }
      return;
      }
   /* Check for AWOL squad */
   success = FALSE;
   for(i = 0; i < 10; i++) if((Quest->marine[i].class != 0) 
    && (Quest->marine[i].level != -1)) success = TRUE;
   if(!success) {
      IconMessage(26);
      DoDelOld();
      return;
      }
   }


ListStats()

{
   int cnt = 55,x,z;
   char astr[30];
   
   SetColor(15);
   RectangleFill(112,55,303,162);
   PrintLineXY("   MP HLT VIT EN ACC DET CRK AMM",112,45);
   SetColor(2);
   MoveTo(18,53);
   DrawTo(302,53);
   PrintLineXY("GEAR 1",214,167);
   for(x = 0; x < 10; x++) {
      if(x == 0) z = 9;
	  else z = x-1;
      if(Quest->marine[z].class != 0) {
	     if(CurMarine == z) itoa(CurMove,astr,10);
	     else itoa(Movements[z],astr,10);
	     PrintLineXY(astr,130+(6*(2-strlen(astr))),cnt);
	     itoa(Quest->marine[z].condition,astr,10);
	     PrintLineXY(astr,148+(6*(3-strlen(astr))),cnt);
	     itoa(Quest->marine[z].vitality,astr,10);
	     PrintLineXY(astr,172+(6*(3-strlen(astr))),cnt);
	     itoa(Quest->marine[z].encumbrance,astr,10);
	     PrintLineXY(astr,196+(6*(2-strlen(astr))),cnt);
	     itoa(Quest->marine[z].shoot,astr,10);
	     PrintLineXY(astr,214+(6*(3-strlen(astr))),cnt);
	     itoa(Quest->marine[z].detect,astr,10);
	     PrintLineXY(astr,238+(6*(3-strlen(astr))),cnt);
	     itoa(Quest->marine[z].crack,astr,10);
	     PrintLineXY(astr,262+(6*(3-strlen(astr))),cnt);
	     itoa(Quest->marine[z].ammo,astr,10);
	     PrintLineXY(astr,286+(6*(3-strlen(astr))),cnt);
	     cnt += 10;
		 }
	  }
   }
   
   

ListGear(num)
int num;

{
   int cnt = 55,x,y,id,z,z1;
   char astr[30];
   int quans[21];
   int max;
   
   max = 11-num;
   num *= 11;
   SetColor(15);
   RectangleFill(112,55,303,162);
   if(num == 0) PrintLineXY("DP PR LA SG SH GS NB CS LP LR SP",112,45);
   else PrintLineXY("CU DE MK AP PC TB FM RK GR RC   ",112,45);
   SetColor(2);
   MoveTo(18,53);
   DrawTo(302,53);
   if(num != 0) PrintLineXY(" STATS ",211,167);
   else PrintLineXY("GEAR 2",214,167);
   for(x = 0; x < 10; x++) {
      if(x == 0) z1 = 9;
	  else z1 = x - 1;
      if(Quest->marine[z1].class != 0) {
         for(id = 0; id < 21; id++) quans[id] = 0;
	     for(id = 0; id < 15; id++) {
	        z = Quest->marine[z1].inventory[id];
		    if(z != 0) quans[(Quest->object[z-1].type - 9) / 2]++;
		    }
         for(y = 0; y < max; y++) {
	        itoa(quans[y+num],astr,10);
	        PrintLineXY(astr,(y*18) + 112 + (6*(2-strlen(astr))),cnt);
		    }
	     cnt += 10;
		 }
	  }
   }
   
   

HiliteMember(ssel)
int ssel;

{
   InvertRect(17,54+ssel*10,110,61+ssel*10);
   }
   
   
   
ListSquad()

{
   int done=0,c,lmode = 0,z;
   int x,cnt = 55;
   char astr[30];
   int ssel;
   int nummar = 0;
   int mardex[10];
   
   ssel = restrict((MousePos.x - 233) / 8,0,9);
   if((iconmap[ssel] == 10) || (Quest->marine[iconmap[ssel]].condition <= 0))
    ssel = 0;
   FlashButton(LX1+2,LY1+2,LX2-2,LY2-2);
   SaveBuffer();
   PageFlip(1);
   SetColor(15);
   RectangleFill(15,30,305,180);
   SetColor(6);
   Rectangle(15L,30L,305L,180L);
   PrintLineXY("Your Squad",130,35);
   PrintLineXY("Name",35,45);
   Rectangle(67L,164L,117L,175L);
   PrintLineXY("OK",86,167);
   Rectangle(137L,164L,187L,175L);
   PrintLineXY("CHANGE",144,167);
   Rectangle(207L,164L,257L,175L);
   ListStats();
   for(x = 0; x < 10; x++) {
      if(x == 0) z = 9;
	  else z = x - 1;
      if(Quest->marine[z].class != 0) {
         strptoc(astr,Quest->marine[z].name);
	     astr[15] = 0;
         PrintLineXY(astr,18,cnt);
	     mardex[nummar] = z;
	     nummar++;
	     cnt += 10;
		 }
	  }
   HiliteMember(ssel);
   PageFlip(0);
   do {
      c = mygetch();
	  if(c == -1) {
	     if(PtInRect(&MousePos,67,164,117,175)) done = 1;
	     else if(PtInRect(&MousePos,137,164,187,175)) done = 2;
	     else if((lmode < 2) && PtInRect(&MousePos,207,164,257,175)) {
		    FlashButton(208,165,256,174);
		    ListGear(lmode);
			lmode++;
			}
	     else if((lmode >= 2)  && PtInRect(&MousePos,207,164,257,175)) {
		    FlashButton(208,165,256,174);
		    ListStats();
			lmode = 0;
			}
	     else if(PtInRect(&MousePos,17,54,110,199)) {
		    z = (MousePos.y - 53) / 10;
			if(z < nummar) {
			   HiliteMember(ssel);
			   ssel = z;
			   HiliteMember(ssel);
			   }
			}
	     }
      else if((c == 2000) && (ssel > 0)) {
	     HiliteMember(ssel);
		 ssel--;
		 HiliteMember(ssel);
		 }
      else if((c == 2002) && (ssel < (nummar-1))) {
	     HiliteMember(ssel);
		 ssel++;
		 HiliteMember(ssel);
		 }
	  else if(c == 'O') done = 1;
	  else if(c == 'C') done = 2;
	  else if((lmode < 2) && (c == 'G')) {
 	     FlashButton(208,165,256,174);
         ListGear(lmode);
		 lmode++;
		 }
	  else if((lmode >= 2) && (c == 'S')) {
		 FlashButton(208,165,256,174);
	     ListStats();
		 lmode = 0;
		 }
	  } while(done == 0);
   if(done == 1) FlashButton(68,165,116,174);
   else FlashButton(138,165,186,174);
   LoadBuffer();
   if(done == 2) {
      z = mardex[ssel];
	  if(Quest->marine[z].vitality <= 0) IconMessage(40);
	  else if(Movements[z] == 0) IconMessage(41);
	  else {
	     PointMarine = z;
		 NextMarine();
		 }
      }
   }