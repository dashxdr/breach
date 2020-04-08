/* breach22.c - Game load and save */

/* Include files */
#include   "breach2.h"

extern long crulen;
extern unsigned char crushed[];
extern Boolean Speedy[10],Invis[10];
extern char SqlInfo[500],GamInfo[500];
extern int SqlLen,GamLen;
extern struct QstStru *Quest;
extern struct PalStru Paladin;
extern Byte obs[5][50][50];
extern Byte mar[5][50][50];
extern Boolean NewFlag,Changed,Quitting,SaveFlag;
extern int MapWindow;
extern int Level;
extern int CurMarine;
extern Boolean Flag137,Flag138;
extern int Movements[10];
extern int CurMove;
extern char GameReply[20];
extern char gname[20],sname[20],lname[13];
extern Point CursorPos;
extern Byte iconmap[10];


Boolean DoCloseMap();


DoSaveOld()

/* Save a GAMe to the disk */

{
   FILE *theFile;
   
   ShapeCursor(1);
   theFile = fopen(GameReply,"w");
   fwrite(gname,1,20,theFile);
   fwrite(sname,1,20,theFile);
   fwrite(lname,1,13,theFile);
   crushit();
   fwrite(crushed,1,(int)crulen,theFile);
   fclose(theFile);
   NewFlag = FALSE;
   Changed = FALSE;
   ShapeCursor(0);
   }
   
   
  
DoNew()

/* Open a new Scenario */

{
   int x,y,z;

   CursorPos.x = 0;
   CursorPos.y = 0;
   MapWindow = 1;
   CreateMarineWindow();
   NewFlag = TRUE;
   Changed = FALSE;
   Flag137 = FALSE;
   Flag138 = FALSE;
   /* Set menus for open file */
   TitleGame();
   DrawTimeWindow();
   for(x = 0; x < 10; x++) {
      Speedy[x] = FALSE;
	  Invis[x] = FALSE;
	  }
   for(x = 1; x < 10; x++) iconmap[x] = 10;
   iconmap[0] = 9;
   z = 1;
   for(x = 0; x < 9; x++) if(Quest->marine[x].class != 0) iconmap[z++] = x;
   CurMarine = 8;
   InitMarinePhase();
   CurMove = Movements[8];
   SetMarL(Quest->PEntry.level,Quest->PEntry.x,Quest->PEntry.y,15);
   NextMarine();
   DrawMapWindow();
   ShapeCursor(0);
   SaveFlag = TRUE;
   }



DoLoader()

{
   int x,y,z;

   /* Set the object matrix */
   if(Quest->PEntry.level != -1)
    Quest->map[Quest->PEntry.level][Quest->PEntry.x]
    [Quest->PEntry.y] = 91;
   if(Quest->PExit.level != -1)
    Quest->map[Quest->PExit.level][Quest->PExit.x]
    [Quest->PExit.y] = 92;
   for(z = 0; z < 5; z++) for(x = 0; x < 50; x++)
    for(y = 0; y < 50; y++) {
      mar[z][x][y] = 15;
      obs[z][x][y] = 0;
      }
   for(x = 0; x < 10; x++)
    if((Quest->marine[x].class != 0) &&
     (Quest->marine[x].condition > 0) &&
     (Quest->marine[x].level >= 0))
     SetMarL(Quest->marine[x].level,Quest->marine[x].x,Quest->marine[x].y,x);
   for(x = 0; x < numobj; x++)
    if((Quest->object[x].type != 0) && (Quest->object[x].held == FALSE))
      obs[Quest->object[x].level][Quest->object[x].x]
       [Quest->object[x].y] = Quest->object[x].type;
   for(x = 0; x < numopp; x++)
    if((Quest->opponent[x].type != 0) &&
     (Quest->opponent[x].health > 0)) {
       obs[Quest->opponent[x].level][Quest->opponent[x].x]
        [Quest->opponent[x].y] = Quest->opponent[x].type+100;
       }
   }



int LoadGame(fname)
char *fname;

/* Load a Game from the disk */

{
   FILE *theFile;
   char s[80];
   int z,x,y;

   strcpy(GameReply,fname);
   theFile = fopen(fname,"r");
   if(theFile == NULL) return(0);
   fread(gname,1,20,theFile);
   fread(sname,1,20,theFile);
   fread(lname,1,13,theFile);
   fread(crushed,1,(int)sizeof(*Quest),theFile);
   fclose(theFile);
   uncrushit();
   NewFlag = FALSE;
   DoLoader();
   SaveFlag = TRUE;
   return(1);
   }
   


LoadScenario(fname)
char *fname;

/* Load a Scenario from the disk */

{
   FILE *theFile;
   char s[80];
   int num,z,x,y;

   theFile = fopen(fname,"r");
   Delay(100L);
   fseek(theFile,21L,0);
   Delay(100L);
   num = fread(crushed,1,(int)sizeof(*Quest),theFile);
   fclose(theFile);
   uncrushit();
   NewFlag = FALSE;
   }
   


SetLeaderStru()


/* Set the leader info structure from the Scenario structure */

{
   int x;

   for(x = 0; x <= 19; x++) Paladin.name[x] = Quest->marine[9].name[x];
   Paladin.class = Quest->marine[9].class;
   Paladin.condition = Quest->marine[9].condition;
   Paladin.vitality = Quest->marine[9].vitality;
   Paladin.maxenc = Quest->marine[9].maxenc;
   Paladin.basemove = Quest->marine[9].basemove;
   Paladin.bonusmove = Quest->marine[9].bonusmove;
   Paladin.shoot = Quest->marine[9].shoot;
   Paladin.crack = Quest->marine[9].crack;
   Paladin.detect = Quest->marine[9].detect;
   Paladin.battles = Quest->battles;
   Paladin.victories = Quest->victories;
   Paladin.wartime = Quest->wartime;
   Paladin.kills = Quest->kills;
   Paladin.lost = Quest->lost;
   }
      
      
      
Boolean DoDelOld()

/* Delete the current GAMe from the disk */

{
   FILE *theFile;
   char SaveReply[80],fname[80],s[80];
   int x,num;
   Boolean canit = FALSE;
   
   /* Save back the squad leader */
   if(Quest->marine[9].condition > 0) {
      do {
         strcpy(fname,Quest->slname);
         strcpy(SaveReply,fname);
         if(num == 0) {
            theFile = NULL;
            canit = TRUE;
            }
         else theFile = fopen(SaveReply,"w");
         if(theFile != NULL) {
            SetLeaderStru();
            num = fwrite(&Paladin,1,sizeof(Paladin),theFile);
            fclose(theFile);
            if(num != sizeof(Paladin)) {
               IconMessage(30);
               fdelete(SaveReply);
               theFile == NULL;
               }
            }
         } while((theFile == NULL) && (!canit));
      }
   fdelete(GameReply);
   Changed = FALSE;
   SaveFlag = TRUE;
   DoCloseMap();
   Quitting = TRUE;
   DrawTimeWindow();
   }
   


SetLInfo(s)
char *s;

{
   int x;
   
   /* Set the leader info in the Scenario structure */
   for(x = 0; x <= 19; x++) Quest->marine[9].name[x] = Paladin.name[x];
   Quest->marine[9].class = Paladin.class;
   Quest->marine[9].condition = 100;
   Quest->marine[9].vitality = 100;
   Quest->marine[9].encumbrance = 0;
   Quest->marine[9].maxenc = Paladin.maxenc;
   Quest->marine[9].basemove = Paladin.basemove;
   Quest->marine[9].bonusmove = Paladin.bonusmove;
   Quest->marine[9].shoot = Paladin.shoot;
   Quest->marine[9].crack = Paladin.crack;
   Quest->marine[9].detect = Paladin.detect;
   Quest->marine[9].ammo = 50;
   if(Paladin.class < 12) {
      Quest->marine[9].armor = 3;
      Quest->marine[9].gun = -1;
      Quest->marine[9].psi = FALSE;
      }
   else {
      Quest->marine[9].armor = 4;
      Quest->marine[9].gun = -1;
      Quest->marine[9].psi = TRUE;
      }
   for(x=0;x < 15; x++) Quest->marine[9].inventory[x] = 0;
   Quest->battles = Paladin.battles;
   Quest->battles++;
   Quest->victories = Paladin.victories;
   Quest->wartime = Paladin.wartime;
   Quest->kills = Paladin.kills;
   Quest->lost = Paladin.lost;
   strcpy(Quest->slname,s);
   DoNGV();
   }
   
   
   
DoNGV()

/* Set up the new GAMe variables */

{
   int x,y,z;
   
   Quest->slkills = 0;
   Quest->slshots = 0;
   Quest->sldetects = 0;
   Quest->slcracks = 0;
   for(z = 0; z < 5; z++) for(x = 0; x < 50; x++)
    for(y = 0; y < 50; y++) mar[z][x][y] = 15;
   for(x = 0; x < numopp; x++)
    Quest->opponent[x].facing = Rnd(8);
   for(x = 0; x < 10; x++) {
      Quest->marine[x].x = -1;
      Quest->marine[x].y = Quest->PEntry.y;
      Quest->marine[x].level = Quest->PEntry.level;
      }
   }
   
   
   
Boolean DoClose()

/* Close windows */

{
   if((MapWindow == NULL) || (DoCloseMap())) return(TRUE);
   return(FALSE);
   }


   
Boolean DoCloseMap()

/* Close a Scenario */

{
   int x;
   
   if(Changed && SaveFlag) {
      x = YesNoBox(52,TRUE);
	  if(x == 1) DoSaveOld();
	  if(x == 3) return(FALSE);
	  }
   if((SaveFlag) || (YesNoBox(49,FALSE) == 1)) {
      Changed = FALSE;
      SaveFlag = TRUE;
      if(MapWindow != NULL) {
         InitCursor();
         MapWindow = NULL;
         }
      return(TRUE);
      }
   return(FALSE);
   }
