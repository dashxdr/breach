/* breach2c.c - Selection Window */

#include "breach2.h"

/* External Variables */
extern int passc;
extern char passv[];
extern char *selmess[];
extern int selwind;
extern char *rankname[];
extern struct QstStru *Quest;
extern struct PalStru Paladin;
extern Point MousePos;
extern int selector[3],counts[3];
extern char scennames[38][22],leadnames[4][20],leadgame[4][20],gamenames[4][20];
extern char gamescen[4][21];
extern char scenfile[38][13],leadfile[4][13],gamefile[4][13],gamelead[4][13];
extern char fpath[80];
extern Byte scendifs[38];
extern char *diffname[4];
extern int sstart;
extern int selup;

/* External Procedures */
extern char *rindex();
extern char *GetCD();
extern char *index();
extern char *itoa();
extern char *strtok();



int SelectYesNo(id)
int id;

{
   char d[255];
   char *e;
   int row = 93,sel = 0,c,cu;

   strcpy(d,selmess[id-1]);
   Save2Buffer();
   SetColor(10);
   RectangleFill(110,75,540,125);
   SetColor(0);
   Rectangle(110L,75L,540L,125L);
   SetColor(4);
   Rectangle(111L,76L,539L,124L);
   SetColor(4);
   Rectangle(134L,109L,234L,120L);
   PrintLineXY("YES",175,112);
   Rectangle(274L,109L,374L,120L);
   PrintLineXY("NO",318,112);
   e = strtok(d,"\n");
   PrintLineXY(e,140,85);
   do {
      e = strtok(NULL,"\n");
      if(e != NULL) PrintLineXY(e,140,row);
      row += 8;
      } while(e != NULL);
   do {
      c = mygetch();
	  if(c > 0) cu = toupper(c);
	  if(cu == 'Y') sel = 1;
	  else if(cu == 'N') sel = 2;
      else if(c == -1) {
         if(PtInRect(&MousePos,134,109,234,120)) {
            sel = 1;
            FlashButton(134,110,234,119);
            }
         if(PtInRect(&MousePos,274,109,374,120)) {
            sel = 2;
            FlashButton(274,110,374,119);
            }
         }
      } while(sel == 0);
   Load2Buffer();
   return(sel);
   }



int GetSkill()

{
   int c,cu,sel = 0;

   Save2Buffer();
   SetColor(10);
   RectangleFill(110,75,540,125);
   SetColor(0);
   Rectangle(110L,75L,540L,125L);
   SetColor(4);
   Rectangle(111L,76L,539L,124L);
   SetColor(4);
   Rectangle(134L,109L,234L,120L);
   PrintLineXY("BEGINNER",160,112);
   Rectangle(274L,109L,374L,120L);
   PrintLineXY("EXPERIENCED",291,112);
   PrintLineXY("Please select a skill level for the mission:",140,85);
   do {
      c = mygetch();
	  if(c > 0) cu = toupper(c);
	  else cu = 0;
	  if(cu == 'B') sel = 1;
	  else if(cu == 'E') sel = 2;
      else if(c == -1) {
         if(PtInRect(&MousePos,134,109,234,120)) {
            sel = 1;
            FlashButton(134,110,234,119);
            }
         if(PtInRect(&MousePos,274,109,374,120)) {
            sel = 2;
            FlashButton(274,110,374,119);
            }
         }
      } while(sel == 0);
   Load2Buffer();
   return(sel-1);
   }



SelectMShow(num)
int num;

{
   char d[255];
   char *e;
   int row = 93;

   strcpy(d,selmess[num-1]);
   Save2Buffer();
   SetColor(10);
   RectangleFill(110,75,540,125);
   SetColor(0);
   Rectangle(110L,75L,540L,125L);
   SetColor(4);
   Rectangle(111L,76L,539L,124L);
   e = strtok(d,"\n");
   PrintLineXY(e,154,85);
   do {
      e = strtok(NULL,"\n");
      if(e != NULL) PrintLineXY(e,154,row);
      row += 8;
      } while(e != NULL);
   }



SelectMClear()

{
   Load2Buffer();
   }



SelectMessage(num)
int num;

{
   SelectMShow(num);
   Rectangle(274L,109L,374L,120L);
   PrintLineXY("OK",318,112);
   KeyWait(); 
   FlashButton(274,110,374,119);
   SelectMClear();
   }



InvertButton(x,y)
int x,y;

{
   InvertRect(x,y,x+69,y+9);
   }



HiliteButton(x,y)
int x,y;

{
   InvertButton(x,y);
   Delay(10L);
   InvertButton(x,y);
   }



HiliteScroll(x,y)
int x,y;

{
   InvertRect(x,y,x+21,y+10);
   Delay(10L);
   InvertRect(x,y,x+21,y+10);
   }



ButtQuit(s)
int *s;

{
   FlashButton(531,13,595,24);
   if(SelectYesNo(5) == 1) *s = 0;
   }



ButtDelScen()

{
   char fname[40];

   HiliteButton(146,190);
   if(selector[0] == -1) SelectMessage(2);
   else if(SelectYesNo(6) == 1) {
      ShapeCursor(1);
      strcpy(fname,scenfile[selector[0]+sstart]);
      fdelete(fname);
      ReadScenarios();
      ListScenarios();
      ShapeCursor(0);
      }
   }




ButtDelGame()

{
   char fname[40];
   
   HiliteButton(512,190);
   if(selector[2] == -1) SelectMessage(1);
   else if(SelectYesNo(13) == 1) {
      ShapeCursor(1);
      strcpy(fname,gamefile[selector[2]]);
      fdelete(fname);
      ReadGames();
      ReadLeaders();
      ListLeaders();
      ListGames();
      ShapeCursor(0);
      }
   }




ButtDelLead()

{
   HiliteButton(511,101);
   if(selector[1] == -1) SelectMessage(3);
   else if(strcmp(leadgame[selector[1]],"Available") != 0)
    SelectMessage(8);
   else if(SelectYesNo(9) == 1) {
      ShapeCursor(1);
	  DeleteLeader(leadfile[selector[1]]);
      ReadLeaders();
      ListLeaders();
      ShapeCursor(0);
      }
   }



ButtAddScen()

{
   Boolean copied;
   
   HiliteButton(61,190);
   if(SelectYesNo(25) == 2) {
      if(SelectYesNo(22) == 1) SwapDisk();
	  return;
	  }
   if(SelectYesNo(7) == 1) {
      copied = CopyScenarios();
      if(copied) ReadScenarios();
      SelectMClear();
      if(copied) ListScenarios();
      ShapeCursor(0);
      }
   }




ButtUpScen()

{
   HiliteScroll(10,29);
   sstart -= 13;
   if(sstart < 0) sstart = 0;
   ListScenarios();
   }



ButtDownScen()

{
   HiliteScroll(254,29);
   if(counts[0] <= (sstart + 13)) return;
   sstart += 13;
   if(sstart > 26) sstart = 26;
   ListScenarios();
   }



CreateGame(gname,skill)
char *gname;
int skill;

{
   char fname[80];
   FILE *theFile;
   int x;

   SelectMShow(17);
   ShapeCursor(1);
   LoadScenario(scenfile[selector[0]+sstart]);
   theFile = fopen(leadfile[selector[1]],"r");
   fread(&Paladin,1,sizeof(Paladin),theFile);
   fclose(theFile);
   SetLInfo(leadfile[selector[1]]);
   Quest->skill = skill;
   for(x = 0; x < 10; x++) Quest->marine[x].gun = -1;
   strcpy(fname,gname);
   fname[8] = '\000';
   strcat(fname,".b2g");
   SetFileName(fname);
  
 
   if(!DoSaveName(gname,&scennames[selector[0]+sstart][1],leadfile[selector[1]],fname)) {
      ShapeCursor(0);
      SelectMessage(4);
	  ShapeCursor(1);
  	  }
   strcpy(fname,leadfile[selector[1]]);
   ReadGames();
   ReadLeaders();
   SelectMClear();
   ListLeaders();
   ListGames();
   if(selector[2] != -1) InvertRect(309,selector[2]*10+140,619,selector[2]*10+149);
   for(x = 0; x < counts[2]; x++) if(strcmp(gamelead[x],fname) == 0) {
      selector[2] = x;
	  InvertRect(309,selector[2]*10+140,619,selector[2]*10+149);
	  break;
	  }
   ShapeCursor(0);
   }



ButtCreGame()

{
   char astr[20];
   int skill;
   
   HiliteButton(342,190);
   if(counts[2] == 4) SelectMessage(12);
   else if(selector[0] == -1) SelectMessage(2);
   else if(selector[1] == -1) SelectMessage(3);
   else if(strcmp(leadgame[selector[1]],"Available") != 0)
    SelectMessage(19);
   else {
      Save2Buffer();
      SetColor(10);
      RectangleFill(100,75,550,125);
      SetColor(0);
      Rectangle(100L,75L,550L,125L);
      SetColor(4);
      Rectangle(101L,76L,549L,124L);
      PrintLineXY("Please enter a name for the game:",140,80);
      SetColor(31);
      astr[0] = '\000';
      EditBox(272,109,16,astr);
	  Load2Buffer();
      if(strlen(astr) > 0) {
	     skill = GetSkill();
	     CreateGame(astr,skill);
		 }
	  }
   }



ButtPlayGame(s)
int *s;

{
   HiliteButton(439,190);
   if(selector[2] == -1) SelectMessage(1);
   else {
      ShapeCursor(1);
      LoadGame(gamefile[selector[2]]);
      *s = 1;
      ShapeCursor(0);
      }
   }



ButtSelScen(dir)
int dir;

{
   int sel;

   if(dir == 0) sel = (MousePos.y - 51) / 10;
   else sel = selector[0] + dir;
   if(sel < 0) return;
   if(sel >= counts[0]-sstart) return;
   if(sel > 12) return;
   if(sel == selector[0]) return;
   if(selector[0] != -1) InvertRect(10,selector[0]*10+51,275,selector[0]*10+60);
   selector[0] = sel;
   InvertRect(10,selector[0]*10+51,275,selector[0]*10+60);
   }



ButtSelLead(dir)
int dir;

{
   int sel;

   if(dir == 0) sel = (MousePos.y - 51) / 10;
   else sel = selector[1] + dir;
   if(sel < 0) return;
   if(sel >= counts[1]) return;
   if(sel == selector[1]) return;
   if(selector[1] != -1) InvertRect(309,selector[1]*10+51,619,selector[1]*10+60);
   selector[1] = sel;
   InvertRect(309,selector[1]*10+51,619,selector[1]*10+60);
   }



ButtSelGame(dir)
int dir;

{
   int sel;

   if(dir == 0) sel = (MousePos.y - 140) / 10;
   else sel = selector[2] + dir;
   if(sel < 0) return;
   if(sel >= counts[2]) return;
   if(sel == selector[2]) return;
   if(selector[2] != -1)
   InvertRect(309,selector[2]*10+140,619,selector[2]*10+149);
   selector[2] = sel;
   InvertRect(309,selector[2]*10+140,619,selector[2]*10+149);
   }


ListScenarios()

{
   int x;

   if(counts[0] == 0) selector[0] = -1;
   else selector[0] = 0;
   SetColor(10);
   RectangleFill(10,51,275,180);
   for(x = sstart; x < sstart+13; x++) {
      if(x >= counts[0]) break;
	  PrintLineXY(&scennames[x][1],15,(x-sstart)*10+53);
	  PrintLineXY(diffname[scendifs[x]],212,(x-sstart)*10+53);
      }   
   if(selector[0] != -1) InvertRect(10,selector[0]*10+51,275,selector[0]*10+60);
   }



ListLeaders()

{
   int x;

   if(counts[1] == 0) selector[1] = -1;
   else if(counts[1] <= selector[1]) selector[1] = counts[1] - 1;
   SetColor(10);
   RectangleFill(309,51,619,90);
   for(x = 0; x < counts[1]; x++) {
      PrintLineXY(leadnames[x],315,x*10+53);
      PrintLineXY(leadgame[x],485,x*10+53);
	  }
   if(selector[1] != -1) InvertRect(309,selector[1]*10+51,619,selector[1]*10+60);
   }



ListGames()

{
   int x;

   if(counts[2] == 0) selector[2] = -1;
   else if(counts[2] <= selector[2]) selector[2] = counts[2] - 1;
   SetColor(10);
   RectangleFill(309,140,619,180);
   for(x = 0; x < counts[2]; x++) {
      PrintLineXY(gamenames[x],315,x*10+142);
      PrintLineXY(gamescen[x],485,x*10+142);
	  }
   if(selector[2] != -1)
    InvertRect(309,selector[2]*10+140,619,selector[2]*10+149);
   }



SetSLDefault()

/* Set defaults for the paladin */

{
   Paladin.class = 5;
   Paladin.condition = 100;
   Paladin.vitality = 100;
   Paladin.maxenc = 25;
   Paladin.basemove = 12;
   Paladin.bonusmove = 0;
   Paladin.shoot = 20;
   Paladin.crack = 15;
   Paladin.detect = 15;
   Paladin.battles = 0;
   Paladin.victories = 0;
   Paladin.wartime = 0;
   Paladin.kills = 0;
   Paladin.lost = 0;
   }
   


SetFileName(fname)
char *fname;

/* Creates a unique file name similar to the parameter fname */

{
   char a = '1';
   char *x;

   x = rindex(fname,'.');
   x--;
   if(access(fname,0) != 0) return;
   else do {
      *x = a;
      a++;
      if(a == '0') {
         a = '1';
         x--;
         }
      } while(access(fname,0) == 0);
   }



ExamineLeader()

{
   char astr[50],bstr[50];
   int x;
   
   Save2Buffer();
   SetColor(8);
   RectangleFill(100,48,350,192);
   SetColor(12);
   Rectangle(100L,48L,350L,192L);
   SetColor(0);
   Rectangle(99L,47L,351L,193L);
   
   strptoc(astr,Paladin.name);
   strcpy(bstr,rankname[Paladin.class-5]);
   strcat(bstr,&astr[2]);
   x = 225 - strlen(bstr) * 3;
   Print2LineXY(bstr,x,55);
   Print2LineXY("Max Encumbrance:",115,73);
   Print2LineXY(itoa(Paladin.maxenc,astr,10),223,73);
   Print2LineXY("Base Move:",115,81);
   Print2LineXY(itoa(Paladin.basemove,astr,10),185,81);
   Print2LineXY("Bonus Move:",115,89);
   Print2LineXY(itoa(Paladin.bonusmove,astr,10),185,89);
   Print2LineXY("Accuracy:",115,97);
   Print2LineXY(itoa(Paladin.shoot,astr,10),185,97);
   Print2LineXY("Cracking:",115,105);
   Print2LineXY(itoa(Paladin.crack,astr,10),185,105);
   Print2LineXY("Detecting:",115,113);
   Print2LineXY(itoa(Paladin.detect,astr,10),185,113);
   Print2LineXY("Detecting:",115,113);
   Print2LineXY(itoa(Paladin.detect,astr,10),185,113);
   Print2LineXY("Battle Stats",125,125);
   SetColor(3);
   MoveTo(202,127);
   DrawTo(310,127);
   DrawTo(310,175);
   DrawTo(105,175);
   DrawTo(105,127);
   DrawTo(120,127);
   Print2LineXY("Victories:",135,140);
   Print2LineXY(itoa(Paladin.victories,astr,10),220,140);
   Print2LineXY("Marines Lost:",135,148);
   Print2LineXY(itoa(Paladin.lost,astr,10),220,148);
   Print2LineXY("Enemy Kills:",135,156);
   Print2LineXY(itoa(Paladin.kills,astr,10),220,156);
   Print2LineXY("Combat Time:",135,164);
   Print2LineXY(itoa((int)(Paladin.wartime/60),astr,10),220,164);
   Print2LineXY("mins.",strlen(astr)*6+226,164);
   SetColor(13);
   Rectangle(200L,178L,250L,189L);
   Print2LineXY("OK",219,181);
	  
   KeyWait();
   FlashButton(200,178,250,189);
   Load2Buffer();
   }
   
   
   
ButtExaLead()

{
   FILE *myfile;

   HiliteButton(426,101);
   if(selector[1] == -1) SelectMessage(3);
   else {
      ShapeCursor(1);
      myfile = fopen(leadfile[selector[1]],"r");
      fread(&Paladin,1,sizeof(Paladin),myfile);
      fclose(myfile);
      ShapeCursor(0);
      ExamineLeader();
      }
   }




ButtCreLead()

{
   char astr[20];

   HiliteButton(341,101);
   if(counts[1] == 4) SelectMessage(10);
   else {
      Save2Buffer();
      SetColor(10);
      RectangleFill(100,75,550,125);
      SetColor(0);
      Rectangle(100L,75L,550L,125L);
      SetColor(4);
      Rectangle(101L,76L,549L,124L);
      PrintLineXY("Please enter a name for the squad leader:",140,80);
      SetColor(31);
      astr[0] = '\000';
      EditBox(272,109,16,astr);
	  Load2Buffer();
      if(strlen(astr) > 0) CreateLeader(astr,TRUE);
      }
   }



HiliteWindow()

{
   if(selwind == 0) InvertRect(34,29,251,39);
   if(selwind == 1) InvertRect(333,29,595,39);
   if(selwind == 2) InvertRect(333,118,595,128);
   }
   
   
   
NextWind()

{
   HiliteWindow();
   selwind++;
   if(selwind > 2) selwind = 0;
   HiliteWindow();
   }
   
   

ButtSel(dir)
int dir;

{
   if(selwind == 0) ButtSelScen(dir);
   if(selwind == 1) ButtSelLead(dir);
   if(selwind == 2) ButtSelGame(dir);
   }
   
   
void DoHelp()

{
   FlashButton(449,13,513,24);
   Save2Buffer();
   SetColor(1);
   RectangleFill(40,35,600,100);
   SetColor(0);
   Rectangle(39L,34L,601L,101L);
   SetColor(4);
   Rectangle(40L,35L,600L,100L);
   PrintLineXY("To begin a new game: First select a scenario to play from the available",100,40);
   PrintLineXY("scenarios window.  Then select a squad leader from the squad leader",100,50);
   PrintLineXY("window.  If the window is empty or none are available you will need to",100,60);
   PrintLineXY("create one.  Then press the make button to create the game.  The game",100,70);
   PrintLineXY("will be displayed in the scenarios in progress window.  Press the play",100,80);
   PrintLineXY("button to begin the game.",100,90);
   KeyWait();
   Load2Buffer();
   }
   
   
   
int DrawSelectWindow()

{
   int x,xu,sel = -1;

   selup = 1;
   FadeOut();
   ShapeCursor(1);
   strcpy(fpath,GetCD());
   if (fpath[strlen(fpath)-1] != ':') strcat(fpath,"/");
   if(passc == 0) {
      ReadScenarios();
      ReadGames();
      ReadLeaders();
      SetupSelect();
      ListScenarios();
      ListLeaders();
      ListGames();
      Fade2In();
	  selwind = -1;
	  NextWind();
      do {
      x = mygetch();
	  if(x > 0) xu = toupper(x);
	  else xu = 0;	  
	  if((x == 9) || (x == ' ') || (x == 13)) NextWind();
	  if(x == 1000) DoHelp();
	  if(x == 1009) SuspendProgram();
	  if(x == 2000) ButtSel(-1);
	  if(x == 2002) ButtSel(1);
	  if(xu == 'R') ButtDelScen();
	  if(xu == 'A') ButtAddScen();
	  if(xu == 'Q') ButtQuit(&sel);
	  if(xu == 'C') ButtCreLead();
	  if(xu == 'E') ButtExaLead();
	  if(xu == 'D') ButtDelLead();
	  if(xu == 'M') ButtCreGame();
	  if(xu == 'P') ButtPlayGame(&sel);
	  if(xu == 'S') ButtDelGame();
	  if(x == 2004) ButtUpScen();
	  if(x == 2005) ButtDownScen();
      if(x == -1) {
         /* Check Scenario Window */
         if(PtInRect(&MousePos,449,13,513,24)) DoHelp();
         else if(PtInRect(&MousePos,146,190,215,199)) ButtDelScen();
         else if(PtInRect(&MousePos,61,190,130,199)) ButtAddScen();
         else if(PtInRect(&MousePos,531,13,595,24)) ButtQuit(&sel);
         else if(PtInRect(&MousePos,10,29,31,39)) ButtUpScen();
         else if(PtInRect(&MousePos,254,29,274,39)) ButtDownScen();
         else if(PtInRect(&MousePos,10,51,275,180)) ButtSelScen(0);
         /* Check Leader Window */
         else if(PtInRect(&MousePos,309,51,619,90)) ButtSelLead(0);
         else if(PtInRect(&MousePos,511,101,580,110)) ButtDelLead();
         else if(PtInRect(&MousePos,341,101,410,110)) ButtCreLead();
         else if(PtInRect(&MousePos,426,101,495,110)) ButtExaLead();
         /* Check Game Window */
         else if(PtInRect(&MousePos,342,190,411,199)) ButtCreGame();
         else if(PtInRect(&MousePos,512,190,581,199)) ButtDelGame();
         else if(PtInRect(&MousePos,439,190,508,199)) ButtPlayGame(&sel);
         else if(PtInRect(&MousePos,309,140,619,180)) ButtSelGame(0);
         }
      } while(sel == -1);
      FadeOut();
	  }
   selup = 0;
   SetupGame();
   return(sel);
   }



