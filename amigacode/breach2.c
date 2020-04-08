/* breach2.c - Main program */

#include   "breach2.h"

Boolean CheckPathMove();
Boolean CheckPathTText();

unsigned char crushed[sizeof(struct QstStru)];
int freq[256][2];
long crulen;
int pow[8] = {1,2,4,8,16,32,64,128};
long bitptr,bytptr;
int passc;
char passv[40];
Byte iconmap[10];
Boolean PulseFlag = FALSE;
long cdel = 8;

Byte maragun[] = {104,92,111,91,113,94,111,97,110,98,104,98,102,95,
                  102,91};
Byte gungun1[] = {6,1,14,2,20,4,11,11,5,12,3,7,3,4,1,5};
Byte gungun2[] = {18,1,23,6,21,10,20,7,17,12,12,11,2,10,10,1};
Byte tankgun[] = {13,0,21,2,22,6,19,13,13,12,6,13,3,6,4,2};
Byte bipedgun1[] = {10,0,18,7,17,6,13,10,10,10,6,8,4,6,5,6};
Byte bipedgun2[] = {13,0,17,5,17,8,15,9,13,10,8,9,4,8,6,3};
Byte robotgun1[] = {5,5,16,4,19,4,17,6,5,7,4,5,4,4,4,4};
Byte robotgun2[] = {18,5,19,5,18,8,21,6,18,5,7,6,5,8,8,4};
Byte aliengun[] = {12,0,15,3,17,4,16,3,12,3,7,3,7,4,8,3};

int quills2[3][4] = {-2,-2,2,-2,0,-3,3,0,2,-2,2,2};
int quills3[3][6] = {-4,-4,0,-4,4,-4,2,-7,4,-4,7,-2,4,-4,4,0,4,4};
int quills4[3][14] = {-2,-1,2,-1,0,-4,-4,-4,4,-4,-2,-5,2,-5,
                      -1,-2,2,1,1,-6,3,-3,6,-1,3,-5,6,-3,
					  1,-2,1,2,4,-4,4,0,4,4,5,-2,5,2};

char *namer[] = {
 "Heavy Wall","Thin Wall","Up Shaft","Down Shaft","Power Door",
 "Steel Door","Glass Door","Open Door","Table","Chair","Controls",
 "Dresser","Bed","Terminal","Machine","Transporter","Fuel Tank",
 "Computer","Worker","Lockers","Blank","Metal Floor","Plain Floor",
 "Deep Water","Shallow Water","Dirt/Water","Grade","High Terrain",
 "Dirt","Grass","Foxhole","Light Veg","Jungle","Forest","Entry",
 "Exit","Occupation","DataPack","Prisoner","Launcher",
 "Smoke Grenade","Shield","GravShoes","Neutron Bomb","Camo Suit",
 "Laser Pistol","Laser Rifle","Stun Pistol","Crack Unit","Detector",
 "MediKit","Ammo Pack","Prox Charge","Tablet","Foxhole Maker","Rocket",
 "Grenade","Remote Charge","Beast","Marauder","Alien",
 "Terradon","Gun Emplacement","Wadgel","Tank","Robot","Biped",
 "Seeker","Stun Field","Rubble"};

/* Mapping of names to elements */
Byte ElmName[] = {
0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
2,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,7,7,8,9,9,10,11,12,13,14,68,15,
16,16,16,17,18,19,20,21,22,
23,24,25,25,25,25,26,26,26,26,27,28,29,30,69,31,32,33,
34,35,36,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
37,37,38,38,39,39,40,40,41,41,42,42,43,43,44,44,45,45,46,46,47,47,48,48,
49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,57};

Byte wallmoves[] =
 {0x1c,0xc1,0x70,0x07,0xFF,0xFF,0xFF,0xFF,0x1c,0xc1,0x70,0x07,0xFF,0xFF,
 0xFF,0xFF,0x00,0x00,
 0xc1,0x07,0x1c,0x70,0xc1,0x07,0x1c,0x70,0xc1,0x07,0x1c,0x70,
 0x40,0x04,0x0c,0x60};

int oppmap[] = {0,188,220,236,252,284,300,316,332,364,376};

Byte spage[43] = {2,3,6,7,8,9,13,14,17,18,20,21,22,23,24,25,26,27,28,29,31,
                  32,34,35,37,38,39,40,41,42,44,45,46,47,49,50,51,52,53,54,
                  55,57,58};

Byte sparagraph[43] = {2,2,3,3,4,3,2,3,1,2,1,3,3,1,2,1,2,2,3,3,1,2,3,3,3,3,
                       3,3,1,3,3,3,2,3,3,1,3,2,1,2,5,2,2};

int scodes[43] = {545,462,534,326,325,564,157,397,429,307,216,421,363,335,593,
                  303,312,619,217,444,456,385,630,217,415,237,293,458,310,629,
                  216,394,286,614,312,225,465,231,350,582,687,314,378};
                  
int letterloc[] = {68,39,38,-1,-1,73,-1,72,69,70,-1,-1,37,41,36,74,35,
                   26,27,28,29,30,31,32,33,34,40,75,-1,-1,-1,71,-1,
				   0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
				   21,22,23,24,25,-1,-1,-1,-1,-1,-1,42,43,44,45,46,47,48,
				   49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67};

char *messages[82] = {
"That marine cannot\ncarry any more objects.",
"Select a rocket in\norder to launch one.",
"That is not a closed door.",
"It takes 15 movement\npoints to use a detector.",
"There is no object\nat that location.",
"There is no room to\ndrop an object here.",
"You must be on a\nshaft.",
"It takes 5 movement\npoints to use a shaft.",
"It takes 3 movement\npoints to shoot.",
"It takes 5 movement\npoints to throw a\ngrenade.",
"It takes 10 movement\npoints to arm a remote\ncharge.",
"It takes 6 movement\npoints to throw a\nneutron bomb.",
"It takes 8 movement\npoints to load an ammo\npack.",
"It takes 9 movement\npoints to use a medikit.",
"It takes 5 movement\npoints to fire a rocket.",
"You must have a launcer\nto fire a rocket.",
"It takes 9 movement\npoints to use a crack\nunit.",
"You must be next to a\ncomputer or terminal to\nuse a crack unit.",
"The crack attempt failed.",
"DataPack vital to mission\nobjective was destroyed.",
"Prisoner vital to mission\nobjective was killed.",
"You have run out of time.",
"You have successfully\ncompleted your mission.",
"Your squad leader has\nbeen killed.",
"Your squad has been\nwiped out.",
"You failed to complete\nthe mission before\nexiting the zone.",
"It takes 10 to use plus\nat least 1 movement point\nto fly GravShoes.",
"The GravShoes crashed.",
"There is no room on\nthe disk.",
"That is not a valid\nfile name.",
"That file does not\nexits.",
"It takes 10 movement\npoints for a psionic\nattack.",
"The psionic attack\nfailed.",
"This marine isn't a\npsionic talent.",
"The shaft is blocked.",
"That is not the\ncorrect word, please\ntry again.",
"That is not the\ncorrect word.  You\ncannot continue.",
"The detect attempt\nfailed.",
"The marine was stunned by\nthe stun field.",
"That marine is stunned.",
"That marine does not have\nany movement points left.",
"Your squad leader's\naccuracy has improved.",
"Your squad leader's\ncracking ability has\nimproved.",
"Your squad leader's\ndetecting ability has\nimproved.",
"Opening a glass door\nrequires 3 movement\npoints.",
"Opening a steel door\nrequires 5 movement\npoints.",
"Opening a power door\nrequires 10 movement\npoints.",
"Checking for squad\nleader improvement...",
"You can only save at the beginning\nof the player phase.  Do you want\nto stop without saving?",
"Cannot find next\nscenario in chain.",
"Moving to next\nencounter area.",
"Save game before stopping?",
"You cannot use a DataPack.",
"You cannot use a Prisoner.",
"It takes 5 movement\npoints to throw a\nsmoke grenade.",
"edit",
"The marine is not facing\nin that direction.",
"You can only save at the\nbeginning of the squad\nleader's turn.",
"There is not enough\nmemory to run Breach 2.",
"You wore the camo suit\nupon taking it.",
"You started using the\nshield upon taking it.",
"It take 20 movement\npoints to dig a\nfoxhole.",
"You cannot dig a\nfoxhole in this type\nof terrain.",
"That marine has not\nreadied a weapon.",
"Are you sure you\nwant to detonate\nall remote charges?",
"To use a proximity\ncharge, just drop\nit.",
"Map cursor is now on.",
"Map cursor is now off.",
"Grid is now on.",
"Grid is now off.",
"Sound is now on.",
"Sound is now off.",
"Your rifle overheated and\nwill not operate until\nnext round.",
"Are you sure you want to\ngo to the next round?",
"Your marine stepped on a\nproximity charge!",
"An opponent stepped on a\nproximity charge!",
"It takes 16 movement\npoints to consume a\ntablet.",
"Combat message speed set\nto fast.",
"Combat message speed set\nto medium.",
"Combat message speed set\nto slow.",
"The marine's gun is out\nof ammo.",
"That object is too heavy\nfor the marine with\nhis current load."};

char *selmess[] = {
"You must select a scenario\nin progress first.",
"You must select a scenario first.",
"You must select a squad leader\nfirst.",
"Not enough disk space to create\na game.",
"Are you sure you want to quit?",
"Are you sure you want to delete\nthe scenario?",
"Are you sure you want to copy\nadditional scenarios from a\nscenario disk?",
"You cannot delete a squad leader\nthat is currently engaged in a\nscenario",
"Are you sure you want to delete\nthe squad leader?",
"You can only have up to 4 squad\nleaders at a time.",
"Not enough disk space to create\na squad leader.",
"You can only have up to 4 saved\ngames at a time.",
"Are you sure you want to delete\nthe game?",
"Insert game disk in DF0:",
"Insert scenario disk in DF0:",
"Not enough disk space to add all\nscenarios.",
"Please wait a moment...",
"That disk is not a\nscenario disk.",
"You must select an available\nsquad leader first.",
"Please remove current disk from\nDF0: and insert another.",
"Do you want to move the selected\nSquad Leader to the new scenario disk?",
"Are you sure you want to\naccess an additional scenario disk?",
"There is no room to save the Squad\nLeader on this\nscenario disk.",
"There are already 4 Squad Leaders on this\ndisk which is the maximum.",
"Are you playing on a hard disk?",
"That Squad Leader is not available and\nwill not be moved."};

char *rankname[] = {"Ensign","2nd Lieutenant","1st Lieutenant","Lt. Commander",
             "Commander","Captain","Commodore","Rear Admiral","Vice Admiral",
			 "Admiral","Fleet Admiral"};
char *rankabbv[] = {"EN","2L","1L","LC","CM","CP","CO","RA","VA","AD","FA"};
Byte rankenc[] = {25,25,27,30,32,34,34,32,30,30,30};
Byte rankbonus[] = {0,2,3,3,4,5,5,6,5,4,4};
Byte rankshoot[] = {20,25,30,40,45,55,60,65,70,75,85};
Byte rankcrack[] = {15,17,20,22,25,30,30,32,34,40,45};
Byte rankdet[] = {15,17,20,22,25,30,30,32,34,40,45};

char SqlInfo[700],GamInfo[700],SceInfo[700];
int SqlLen,GamLen,SceLen;

extern char *itoa();
Boolean BitTst();
Boolean CheckMove();

int selector[3],counts[3];
char scennames[38][21],leadnames[4][20],leadgame[4][20],gamenames[4][20];
char gamescen[4][21];
char scenfile[38][13],leadfile[4][13],gamefile[4][13],gamelead[4][13];
char fpath[80];
Byte scendifs[38];
char *diffname[4] = {"Easy","Medium","Hard","V Hard"};
int sstart;
Boolean saveit,GridFlag = FALSE,SoundFlag = TRUE;

char GameReply[20];
char gname[20],sname[20],lname[13];
Byte see[17][17],Heater[10];
Boolean Speedy[10],Confuse,CursorFlag,Invis[10],InvisView[10];
Boolean SkipIt = FALSE,Scaned = FALSE;
Boolean modifier = FALSE,optioned = FALSE;
int MapWindow = NULL;
Boolean Quitting = FALSE, Finished = FALSE,SaveFlag = TRUE;
int Level = 0;
struct QstStru *Quest;
struct PalStru Paladin;
Point MapOrigin,MapOld;
Boolean WholeFlag = FALSE,NewFlag,Changed = FALSE,wasDown,DoOff;
Boolean ScaleFlag = FALSE,BeltFlag = FALSE,QToFlag = FALSE;
Boolean Flag137,Flag138,outrng;
int IconFlag = 0,ArmFlag = -1;
long ScenLen;
char LoadReply[80];
int facing;
int PointFlag = 0;
int pows[] = {1,2,4,8,16};
Byte passer[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,2,1,1,
2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
Byte viewer[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,
1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
Byte maskinfo[] = {
24,0,25,0,0,0,0,0,32,0,33,0,0,0,0,0,
0,0,0,0,32,33,0,0,32,33,0,0,32,33,0,0,51,0,53,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,106,0,0,0,0,0,0,0,0,0,0,0,106,0,0,0,0,0,0,0};

Byte movc[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,
5,5,2,2,2,2,0,0,2,2,0,0,2,2,2,2,2,0,2,0,5,4,4,5,6,5,5,6,8,6,0,0,0,5,6,6,2,3,2,
0,4,3,3,3,3,5,5,5,5,3,2,2,6,4,4,9,6,3,3,2};

Byte WholeColor[] = {
 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
 4,4,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,3,3,3,3,14,4,14,14,14,3,3,3,0,7,11,
 1,9,9,9,9,9,6,6,6,6,6,12,10,14,12,6,2,2,4,4,4};

Byte cdata[6] = {11,37,7,41,67,71};
Byte obs[5][50][50];
Byte mar[5][50][50];
int CurMarine,CurMove;
int Movements[10];
int rnum;
int PointMarine = -1;
extern int DemoLevel;
long RowCnt;
Boolean NEFlag;

Point MapSelect;
Point MousePos,CursorPos;
Boolean fn;
char q;
int selwind = -1;


struct nodestru *finnode,*tree;
struct pathstru path[2][169];
int CurEnemy,curpath,curx,cury,destx,desty,targx,targy;
Byte fill[13][13];
int pquan[2];
int tnum,trng,tdir;
Boolean arrived,lifted;

int NameCount;
int mnum[15],mtyp[15];

int tpath[4][4][4][2] = {
   1,-1,0,0,0,0,0,0,
   0,-1,1,0,0,-1,0,0,
   0,-1,1,-1,0,-1,0,0,
   0,-1,0,-1,1,-1,0,-1,
   1,-1,1,0,0,0,0,0,
   1,-1,1,-1,0,0,0,0,
   1,-1,0,-1,1,-1,0,0,
   0,-1,1,-1,0,-1,1,-1,
   1,0,1,-1,1,0,0,0,
   1,-1,1,0,1,-1,0,0,
   1,-1,1,-1,1,-1,0,0,
   1,-1,0,-1,1,-1,1,-1,
   1,0,1,0,1,-1,1,0,
   1,0,1,-1,1,0,1,-1,
   1,-1,1,0,1,-1,1,-1,
   1,-1,1,-1,1,-1,1,-1};
   

char *obna[] = {"DataPack","Prisoner","Launcher",
 "Smoke Grenade","Shield","GravShoes","Neutron Bomb","Camo Suit",
 "Laser Pistol","Laser Rifle","Stun Pistol","Crack Unit","Detector",
 "MediKit","Ammo Pack","Prox Charge","Tablet","Foxhole Maker","Rocket",
 "Grenade","Remote Charge"};
Byte objenc[] = {1,1,7,1,2,4,2,5,1,2,1,9,9,3,2,5,1,7,3,1,5,5};

int CurItem,Start,theItem;
Boolean DropFlag,LaunchFlag;
int guns[] = {25,55,65,90,100,100,100};
int suits[] = {15,30,40,65};
int CurIndex;
int DemoLevel;

Byte targimg[6][4] = {9,3,13,3,11,4,11,14,9,15,13,15,4,7,4,11,5,9,17,9,18,7,18,11};
Byte targmask[29][2] = {8,2,14,2,14,4,12,4,12,8,17,8,17,6,19,6,19,12,17,12,17,10,
                        12,10,12,14,14,14,14,16,8,16,8,14,10,14,10,10,5,10,5,12,
                        3,12,3,6,5,6,5,8,10,8,10,4,8,4,8,2};


int GetMarL(l,x,y)
int l,x,y;

{
   return((int)(mar[l][x][y] & 0x0F));
   }
   
   
   
int GetMarH(l,x,y)
int l,x,y;

{
   return((int)(mar[l][x][y] >> 4));
   }
   
   
   
SetMarH(l,x,y,v)
int l,x,y,v;

{
   mar[l][x][y] = (mar[l][x][y] & 0x0F) | (v << 4);
   }
   
   
   
SetMarL(l,x,y,v)
int l,x,y,v;

{
   mar[l][x][y] = (mar[l][x][y] & 0xF0) | v;
   }
   
   
   
PlaySelected()

/* Play the game selected in the select screen */

{
   int x;
   
   if(passc > 0) {
      x = LoadGame(passv);
      if(x == 0) return;
	  }
   Quitting = FALSE;
   DrawGameScreen();
   DoNew();
   do {
      DoEvent();
      } while(!Finished);
   EraseGameScreen();
   }
   
   

main(argc,argv)
int argc;
char *argv[];

/* Map Maker main procedure */

{
   int sel;

   if(argc > 0) passc = argc - 1;
   else passc = 0;
   if(passc > 0) strcpy(passv,argv[1]);
   if(Initialize() != 0) do {
      sel = DrawSelectWindow();
      if((sel == 1) || (passc > 0)) PlaySelected();
      } while((sel == 1) && (passc == 0));
   Terminate();
   }



Boolean PtInRect(pt,x1,y1,x2,y2)
int x1,y1,x2,y2;
Point *pt;

{
   if((pt->x < x1) || (pt->x > x2) || (pt->y < y1) || (pt->y > y2))
    return(FALSE);
   return(TRUE);
   }



void KeyCursor()

{
   int x,y,z;
   
   x = CursorPos.x * 24;
   y = CursorPos.y * 18 + 16;
   SetColor(2);
   for(z = 0; z < 6; z++) {
      MoveTo(targimg[z][0]+x,targimg[z][1]+y);
      DrawTo(targimg[z][2]+x,targimg[z][3]+y);
      }
   SetColor(0);
   MoveTo(8+x,2+y);
   for(z = 1; z < 29; z++) DrawTo(targmask[z][0]+x,targmask[z][1]+y);
   }
   
   
   
MoveCursor(c)
int c;

{
   int x,y;
   
   if(!CursorFlag) return;
   x = CursorPos.x;
   y = CursorPos.y;
   if(c == 2000) CursorPos.y--;
   if(c == 2001) CursorPos.x++;
   if(c == 2002) CursorPos.y++;
   if(c == 2003) CursorPos.x--;
   if(c == 2004) { CursorPos.x++; CursorPos.y--; }
   if(c == 2005) { CursorPos.x++; CursorPos.y++; }
   if(c == 2006) { CursorPos.x--; CursorPos.y++; }
   if(c == 2007) { CursorPos.x--; CursorPos.y--; }
   CursorPos.x = restrict(CursorPos.x,0,8);
   CursorPos.y = restrict(CursorPos.y,0,8);
   SavePartial();
   if(see[CursorPos.x+4][CursorPos.y+4]) DrawSquare(CursorPos.x,CursorPos.y,1);
   else BlitElement(70,CursorPos.x,CursorPos.y,1,srcCopy);
   if(PointFlag == 0) {
      BlitElement(102,CursorPos.x,CursorPos.y,1,notSrcBic);
      BlitElement(101,CursorPos.x,CursorPos.y,1,srcOr);
      }
   if((x != CursorPos.x) || (y != CursorPos.y)) {
      if(see[x+4][y+4]) DrawSquare(x,y,1);
      else BlitElement(70,x,y,1,srcCopy);
	  }
   LoadPartial();
   if(PointFlag != 0) KeyCursor();
   SkipIt = TRUE;
   }
   
   
   
void ImmMove(c)
int c;

{
   int x=4,y=4;
   int dx,dy,cost,f;
   int diag,curelm;

   modifier = FALSE;
   Changed = TRUE;
   if(c == 2000) { y--; f = 0; }
   if(c == 2001) { x++; f = 2; }
   if(c == 2002) { y++; f = 4; }
   if(c == 2003) { x--; f = 6; }
   if(c == 2004) { y--; x++; f = 1; }
   if(c == 2005) { y++; x++; f = 3; }
   if(c == 2006) { y++; x--; f = 5; }
   if(c == 2007) { y--; x--; f = 7; }
   diag = f & 1;

   if(Quest->marine[CurMarine].facing != f) {
      ChangeFacing(f);
      DrawMarineWindow();
      return;
	  }

   dx = x + MapOrigin.x;
   dy = y + MapOrigin.y;
   if((dx < 0) || (dy < 0) || (dx > 49) || (dy > 49)) return;

   curelm = Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4];
   if(!CheckPathMove(curelm,f,0,(Boolean)diag,Level,x,y)) return;

   if(BeltFlag) {
      cost = 1;
      if(diag) cost++;
      }
   else {
      cost = movc[Quest->map[Level][dx][dy]];
      if(diag) cost = cost / 2;
      }
   CurMove -= cost;
   SetMarL(Level,MapOrigin.x+4,MapOrigin.y+4,15);
   MapOrigin.x += x - 4;
   MapOrigin.y += y - 4;
   Quest->marine[CurMarine].x = MapOrigin.x+4;
   Quest->marine[CurMarine].y = MapOrigin.y+4;
   SetMarL(Level,MapOrigin.x+4,MapOrigin.y+4,CurMarine);
   CheckPathTText(MapOrigin.x+4,MapOrigin.y+4,Level);
   CheckPathStun();
   DrawMapWindow();
   DrawMarineWindow();
   }



void ImmMoveMouse()

{
   int dx,dy;

   ConvertMouseXY(&dx,&dy);
   dx -= 4;
   dy -= 4;
   if(dx == 0) {
      if(dy == -1) ImmMove(2000);
      if(dy == 1) ImmMove(2002);
      }
   if(dx == 1) {
      if(dy == -1) ImmMove(2004);
      if(dy == 1) ImmMove(2005);
      if(dy == 0) ImmMove(2001);
      }
   if(dx == -1) {
      if(dy == -1) ImmMove(2007);
      if(dy == 1) ImmMove(2006);
      if(dy == 0) ImmMove(2003);
      }
   modifier = FALSE;
   }
   
   
   
DoEvent()

{
   int x,i,j,k,c,cu;
   long templ;

   do {
      PulseFlag = TRUE;
      c = mygetch();
	  if(c != 0) PulseFlag = FALSE;
	  if(c > 0) cu = toupper(c);
	  else cu = 0;
	  if(IconFlag == 8) {
	     if(c == 13) c = 90;
	     if(((c > 0) && (c < 2000) && (c != 'Z')) || (c > 2007)) ResetIcons();
		 }
      if(c == -3) SkipIt = TRUE;
	  if(c == 0) SkipIt = TRUE;
	  if(c == -2) ImmMoveMouse();
      if(c == -1) {
	     saveit = FALSE;
         if((PtInRect(&MousePos,0,16,215,177)) && (MapWindow != NULL))
		  DoMouseMap(TRUE);
         else if((PtInRect(&MousePos,RX1,RY1,RX2,RY2)) && (MapWindow !=
	      NULL)) DoDone(TRUE);
         else if((PtInRect(&MousePos,OX1,OY1,OX2,OY2)) && (MapWindow !=
	      NULL)) DoStatus();
         else if((PtInRect(&MousePos,LX1,LY1,LX2,LY2)) && (MapWindow !=
	      NULL)) ListSquad();
         else if((PtInRect(&MousePos,BX1,BY1,BX2,BY2)) && (MapWindow !=
	      NULL)) DoBriefing();
         else if((PtInRect(&MousePos,DX1,DY1,DX2,DY2)) && (MapWindow !=
	      NULL)) SetIcons(0,TRUE);
         else if((PtInRect(&MousePos,SX1,SY1,SX2,SY2)) && (MapWindow !=
	      NULL)) SetIcons(1,TRUE);
         else if((PtInRect(&MousePos,TX1,TY1,TX2,TY2)) && (MapWindow !=
	      NULL)) SetIcons(2,TRUE);
         else if((PtInRect(&MousePos,UX1,UY1,UX2,UY2)) && (MapWindow !=
	      NULL)) SetIcons(3,TRUE);
         else if((PtInRect(&MousePos,KX1,KY1,KX2,KY2)) && (MapWindow !=
	      NULL)) SetIcons(4,TRUE);
         else if((PtInRect(&MousePos,AX1,AY1,AX2,AY2)) && (MapWindow !=
	      NULL)) SetIcons(5,TRUE);
         else if((PtInRect(&MousePos,IX1,IY1,IX2,IY2)) && (MapWindow !=
	      NULL)) SetIcons(7,TRUE);
         else if((PtInRect(&MousePos,FX1,FY1,FX2,FY2)) && (MapWindow !=
	      NULL)) {
            FlashButton(FX1+2,FY1+2,FX2-2,FY2-2);
	        Quitting = TRUE;
		    }
         else if((PtInRect(&MousePos,NX1,NY1,NX2,NY2)) && (MapWindow !=
	      NULL)) DoDone(FALSE);
         else if((PtInRect(&MousePos,WX1,WY1,WX2,WY2)) && (MapWindow != NULL)) DoScale();
         else SkipIt = TRUE;
		 }
	  else if(c == 1005) {
	     CursorFlag = !CursorFlag;
		 if(MapWindow != NULL) DrawMapWindow();
		 if(CursorFlag) IconMessage(67);
		 else IconMessage(68);
		 }
	  else if(c == 1006) {
	     GridFlag = !GridFlag;
		 if(MapWindow != NULL) DrawMapWindow();
		 if(GridFlag) IconMessage(69);
		 else IconMessage(70);
		 }
	  else if(c == 1007) {
	     SoundFlag = !SoundFlag;
		 if(SoundFlag) IconMessage(71);
		 else IconMessage(72);
		 }
	  else if(c == 1008) {
	     cdel += 50;
		 if(cdel > 108) cdel = 8;
		 IconMessage((int)((cdel-8) / 50 + 78));
		 }
	  else if(c == 1009) SuspendProgram();
  	  else if(c == 1000) { 
	     Quitting = TRUE;
         FlashButton(FX1+2,FY1+2,FX2-2,FY2-2);
		 }
	  else if(c == 90) DoMouseMap(FALSE);
	  else if((c >= 2000) && (c <= 2007)) {
	     if(!modifier) MoveCursor(c);
		 else ImmMove(c);
		 }
      else if(cu == 'B') DoBriefing();
	  else if(cu == 'O') DoStatus();
	  else if(cu == 'N') DoDone(FALSE);
	  else if(cu == 'R') DoDone(TRUE);
	  else if(cu == 'L') ListSquad();
	  else if(cu == 'W') DoScale();
	  else if(cu == 'D') SetIcons(0,FALSE);
	  else if(cu == 'S') SetIcons(1,FALSE);
	  else if(cu == 'T') SetIcons(2,FALSE);
	  else if(cu == 'U') SetIcons(3,FALSE);
	  else if(cu == 'K') SetIcons(4,FALSE);
	  else if(cu == 'A') SetIcons(5,FALSE);
	  else if(cu == 'I') SetIcons(7,FALSE);
	  else if(cu == 'P') TracePath(FALSE);
      else SkipIt = TRUE;
      if((IconFlag != 0) && (!SkipIt)) ResetIcons();
      if((PointFlag != 0) && (!SkipIt)) {
         PointFlag = 0;
         ShapeCursor(0);
         MoveCursor(0);
         }
       SkipIt = FALSE;
	   if(Quitting) Quitting = DoClose();
       } while(!Quitting);
   Finished = TRUE;
   }



SeePoint()

/* Look for a pointed marine */

{
   int x,y,z;
   
   ConvertMouseXY(&x,&y);
   x += MapOrigin.x;
   y += MapOrigin.y;
   z = GetMarL(Level,x,y);
   if((z != 15) && (Quest->marine[z].class != 0)) {
      if(Quest->marine[z].vitality <= 0) IconMessage(40);
      else if(Movements[z] == 0) IconMessage(41);
      else {
         PointMarine = z;
         NextMarine();
         }
      }
   }
   


DoDone(round)
Boolean round;

{
   if(round) {
     FlashButton(RX1+2,RY1+2,RX2-2,RY2-2);
     if((Quest->skill == 0) && (YesNoBox(74,FALSE) != 1)) return;
	  }
   else FlashButton(NX1+2,NY1+2,NX2-2,NY2-2);
   modifier = round;
   NextMarine();
   }



InitMoves(x)
int x;

{
   int y,z;
   
   if((Quest->marine[x].class != 0) &&
    (Quest->marine[x].x != -1) &&
    (Quest->marine[x].level != -1)) {
      Movements[x] = (Quest->marine[x].basemove + 
       Quest->marine[x].bonusmove + 8)
       - ((Quest->marine[x].encumbrance - 2) / 2);
      if(Quest->marine[x].vitality < 75)
       Movements[x] *= (Quest->marine[x].vitality * 2);
      else Movements[x] *= 150;
      Movements[x] /= 100;
      if(Speedy[x]) Movements[x] += 15;
      /* Set vitality */
      Quest->marine[x].vitality +=
       Quest->marine[x].condition / 4;
      if(Quest->marine[x].vitality > 100)
       Quest->marine[x].vitality = 100;
      }
   else Movements[x] = 0;
   Speedy[x] = FALSE;
   Heater[x] = 0;
   SetInvis(x);
   }
   
   

ClearSmoke()

{
   int x,y,z,g;

   for(x = 0; x < 50; x++) for(y = 0; y < 50; y++) for(z = 0; z < 5; z++) {
      g = GetMarH(z,x,y);
	  if(g > 0) SetMarH(z,x,y,g-1);
	  }
   }
   
   
   
InitMarinePhase()

{
   int x;
   
   ClearSmoke();
   for(x = 0; x <= 9; x++) InitMoves(x);
   Scaned = FALSE;
   Confuse = FALSE;
   }
   
   

Boolean NoMarines()

/* returns TRUE if there are no marines to move */

{
   int x;
   
   if(GetMarL(Quest->PEntry.level,Quest->PEntry.x,Quest->PEntry.y) == 15) 
    for(x = 0; x <= 9; x++)
    if((Quest->marine[x].x == -1) &&
    (Quest->marine[x].class != 0)) return(FALSE);
   for(x = 0; x <= 9; x++) if(Movements[x] > 0) return(FALSE);
   return(TRUE);
   }
   
   
FindNextMarine()

{
   Boolean NextFlag = FALSE;
   Byte z;
   
   if(modifier) NextFlag = TRUE;
   if(CurMarine >= 0) Movements[CurMarine] = CurMove;
   if(PointMarine != -1) {
      CurMarine = PointMarine - 1;
      PointMarine = -1;
      }
   do {
      CurMarine++;
      if(CurMarine > 9) CurMarine = 0;
      if((Quest->marine[CurMarine].x == -1) &&
       (!(NextFlag)) &&
       (Quest->marine[CurMarine].class != 0) &&
       (GetMarL(Quest->PEntry.level,Quest->PEntry.x,Quest->PEntry.y) == 15)) {
         Quest->marine[CurMarine].x = Quest->PEntry.x;
         InitMoves(CurMarine);
		 SetMarL(Quest->PEntry.level,Quest->PEntry.x,Quest->PEntry.y,CurMarine);
         Level = Quest->PEntry.level;
         Quest->marine[CurMarine].facing = (CurMarine % 8);
         if(obs[Level][Quest->PEntry.x]
          [Quest->PEntry.y] >= 100) KillNEEnemy(FindEnemy(
          Quest->PEntry.x,Quest->PEntry.y));
         }
      /* Get rid of all mouse down events before changing */
      if(NextFlag || NoMarines()) {
         modifier = FALSE;
         CurMarine = 20;
         Quest->TimeLimit -= 30;
         Quest->wartime += 30;
         DrawTimeWindow();
         DoEnemies();
		 while(mygetch() != 0);
         EndGame(0);
         InitMarinePhase();
         CurMarine = 9;
		 saveit = TRUE;
         SaveFlag = TRUE;
         NextFlag = FALSE;
         }
      } while(((Quest->marine[CurMarine].class == 0) ||
              (Quest->marine[CurMarine].x == -1) ||
              (Quest->marine[CurMarine].level == -1)) &&
              (!Quitting));
   CurMove = Movements[CurMarine];
   }
   
   
   
NextMarine()

/* Go to the next marine in the list */

{
   int z;
   
   if((CurMarine >= 0) && (!Quitting)) {
      if(BeltFlag) BeltOff();
      if((Quest->marine[CurMarine].level == Quest->PExit.level)
       && (Quest->marine[CurMarine].x == Quest->PExit.x)
       && (Quest->marine[CurMarine].y == Quest->PExit.y)) {
        SetMarL(Level,Quest->marine[CurMarine].x,Quest->marine[CurMarine].y,15);
        Quest->marine[CurMarine].level = -1;
        CurMove = 0;
        }
      EndGame(0);
      }
   if(!Quitting) do {
      FindNextMarine();
      } while((CurMove == 0) && (!Quitting));
   if(!Quitting) {
      DrawMarineWindow();
      Level = Quest->marine[CurMarine].level;
      SetPt(&MapOrigin,Quest->marine[CurMarine].x - 4,
       Quest->marine[CurMarine].y - 4);
      SetMarL(Level,4+MapOrigin.x,4+MapOrigin.y,CurMarine);
      DrawMapWindow();
      /* Hilight the new marine */
      SaveBuffer();
      SetColor(1);
      Rectangle(96L,88L,119L,105L);
      Rectangle(97L,89L,118L,104L);
      Delay(13L);
      LoadBuffer();
      if(CurMarine == 9) SaveFlag = TRUE;
      }
   }
   


TitleGame()

{
   PrintLineXY(gname,108-strlen(gname)*3,2);
   BlitInsignia(Quest->marine[9].class - 5,1,1);
   BlitInsignia(Quest->marine[9].class - 5,191,1);
   }



TitleMarine(s)
char *s;

{
   char d[30];
   int x;

   SetColor(15);
   RectangleFill(222,52,318,59);
   strptoc(d,s);
   d[16] = '\000';
   x = (16-strlen(d)) * 3 + 223;
   PrintLineXY(d,x,53);
   }



WarnBox(x,tog)
int x;
Boolean tog;

{
   if(tog) SetColor(13);
   else SetColor(15);
   Rectangle(277L,(long)(x-2),315L,(long)(x+7));
   }
   
   
   
DrawMarineWindow()

{
   char astr[10];

   if(MapWindow != NULL) {
      if((Quest->marine[CurMarine].vitality <= 0)
       && (MapWindow != NULL)) {
         Quest->marine[CurMarine].vitality = 0;
         CurMove = 0;
         }
      if((CurMove <= 0) && (MapWindow != NULL)) NextMarine();
      if(MapWindow != NULL) {
         TitleMarine(Quest->marine[CurMarine].name);
         itoa(CurMove,astr,10);
		 strcat(astr,"  ");
		 PrintLineXY(astr,280,65);
         if(Quest->marine[CurMarine].vitality < 30) WarnBox(75,TRUE);
		 else WarnBox(75,FALSE);
         itoa(Quest->marine[CurMarine].vitality,astr,10);
		 strcat(astr,"%  ");
		 PrintLineXY(astr,280,75);
         if(Quest->marine[CurMarine].condition < 50) WarnBox(85,TRUE);
		 else WarnBox(85,FALSE);
         itoa(Quest->marine[CurMarine].condition,astr,10);
		 strcat(astr,"%  ");
		 PrintLineXY(astr,280,85);
         itoa(Quest->marine[CurMarine].encumbrance,astr,10);
		 strcat(astr,"  ");
		 PrintLineXY(astr,280,95);
         itoa(Quest->marine[CurMarine].shoot,astr,10);
		 strcat(astr,"%  ");
		 PrintLineXY(astr,280,105);
         itoa(Quest->marine[CurMarine].detect,astr,10);
		 strcat(astr,"%  ");
		 PrintLineXY(astr,280,115);
         itoa(Quest->marine[CurMarine].crack,astr,10);
		 strcat(astr,"%  ");
		 PrintLineXY(astr,280,125);
         itoa(Quest->marine[CurMarine].ammo,astr,10);
		 strcat(astr,"   ");
		 PrintLineXY(astr,280,135);
         }
       }
   }



DrawTimeWindow()

{
   char astr[5],bstr[5],cstr[5],d[10];
   int Hour,Min,Sec;

   /* Draw the time window */
   if(MapWindow == NULL) PrintLineXY("00:00:00",264,2);
   else {
      Hour = Quest->TimeLimit / 3600;
      Min = (Quest->TimeLimit - (Hour * 3600));
      Sec = Min % 60;
      Min /= 60;
      itoa(Hour,astr,10);
      itoa(Min,bstr,10);
      itoa(Sec,cstr,10);
      if(strlen(astr) < 2) strcpy(d,"0");
      else strcpy(d,"");
      strcat(d,astr);
      if(strlen(bstr) < 2) strcat(d,":0");
      else strcat(d,":");
      strcat(d,bstr);
      if(strlen(cstr) < 2) strcat(d,":0");
      else strcat(d,":");
      strcat(d,cstr);
      PrintLineXY(d,264,2);
      }
   }



BlitSemi(elm,x,y)
int elm,x,y;

{
   int x1,y1;

   x1 = x * 10 + 3;
   y1 = y * 7 + 23;
   SetColor(WholeColor[elm-18]);
   RectangleFill(x1,y1,x1+9,y1+6);
   }



BlitScan(x,y)
int x,y;

{
   int x1,y1;

   x1 = x * 10 + 3;
   y1 = y * 7 + 23;
   SetColor(13);
   RectangleFill(x1,y1,x1+9,y1+6);
   }



BlitProx(x,y)
int x,y;

{
   int x1,y1;

   x1 = x * 10 + 3;
   y1 = y * 7 + 23;
   SetColor(5);
   RectangleFill(x1,y1,x1+9,y1+6);
   }



DoScale()

{
   int x,y,x1,y1;
   
   FlashButton(WX1+2,WY1+2,WX2-2,WY2-2);
   SetColor(15);
   RectangleFill(0,16,215,177);
   for(x = 0; x < 21; x++) for(y = 0; y < 21; y++) {
      x1 = x+MapOrigin.x-6;
      y1 = y+MapOrigin.y-6;
      if((x1 < 0) || (x1 > 49) || (y1 < 0) || (y1 > 49)) BlitSemi(70,x,y);
      else if(BitTst(&Quest->vis[x1][y1],(long)Level)) {
         if((obs[Level][x1][y1] >= 100) &&
          (Quest->marine[CurMarine].class == 3))
          BlitScan(x,y);
         else if((obs[Level][x1][y1] == 39) && (Rnd(100) < 40) &&
          (Quest->marine[CurMarine].class == 3))
          BlitProx(x,y);
         else if((x == 10) && (y == 10)) BlitSemi(70,x,y);
         else BlitSemi(Quest->map[Level][x1][y1],x,y);
         }
      }
   KeyWait();
   DrawMapWindow();
   }



DoWhole()

{
   int x,y;
   int x1,y1;
   Boolean BStat;

   SoundEff(19);
   SetColor(15);
   RectangleFill(0,16,215,177);
   x1 = 4;
   for(x = 0; x <= 49; x++) {
       x1 += 4;
       y1 = 19;
       for(y = 0; y <= 49; y++) {
          y1 += 3;
          if(BitTst(&Quest->vis[x][y],(long)Level)) {
             SetColor(WholeColor[Quest->map[Level][x][y]]);
             RectangleFill(x1,y1,x1+3,y1+2);
             }
          }
       }
   KeyWait();
   SoundEff(-1);
   }



CreateMarineWindow()

{
   if(MapWindow != NULL) {
      SetColor(15);
      RectangleFill(221,51,319,144);
      SetColor(1);
      Rectangle(220L,51L,319L,144L);
      MoveTo(220,60);
      DrawTo(319,60);
	  PrintLineXY("Moves:",235,65);
	  PrintLineXY("Vital:",235,75);
	  PrintLineXY("Health:",229,85);
	  PrintLineXY("Encumb:",229,95);
	  PrintLineXY("Accur:",235,105);
	  PrintLineXY("Detect:",229,115);
	  PrintLineXY("Crack:",235,125);
	  PrintLineXY("Ammo:",241,135);
	  }
   }



ShootGun(x,y)
int x,y;

/* Fire the current marine's gun a square x,y */

{
   int dx,dy,adx,ady;
   Boolean valid = FALSE;

   dx = x - 4;
   dy = y - 4;
   adx = abs(dx);
   ady = abs(dy);
   switch(Quest->marine[CurMarine].facing) {
      case 0: if((dy < 0) && (ady >= adx)) valid = TRUE; break;
      case 1: if((dy <= 0) && (dx >= 0)) valid = TRUE; break;
      case 2: if((dx > 0) && (adx >= ady)) valid = TRUE; break;
      case 3: if((dy >= 0) && (dx >= 0)) valid = TRUE; break;
      case 4: if((dy > 0) && (ady >= adx)) valid = TRUE; break;
      case 5: if((dy >= 0) && (dx <= 0)) valid = TRUE; break;
      case 6: if((dx < 0) && (adx >= ady)) valid = TRUE; break;
      case 7: if((dy <= 0) && (dx <= 0)) valid = TRUE; break;
      }
   if(valid) 
    switch(Quest->marine[CurMarine].gun) {
	  case -1: IconMessage(64); break;
	  case 0:
      case 1:
	  case 2: DoShoot(x,y); break;
	  case 3: DoPsiCast(x,y); break;
  	  }
   else IconMessage(57);
   }



ConvertMouseXY(x,y)
int *x,*y;

{
   *x = MousePos.x / 24;
   *y = (MousePos.y - 16) / 18;
   }
   
   
   
DoMouseMap(mflag)
Boolean mflag;

{
   int x,y;
  
   if(!mflag) SetPt(&MousePos,CursorPos.x*24+12,CursorPos.y*18+24);
   if(PointFlag != 0) DoItem();
   else if(IconFlag != 0) DoIcons();
   else { 
      if(optioned) SeePoint();
      else if(modifier) ImmMoveMouse();
      else {
         ConvertMouseXY(&x,&y);
         if((x == 4) && (y == 4)) {
		    if(mflag) TracePath(TRUE);
			}
         else ShootGun(x,y);
         }
      }
   }



int FindOpponent(x,y)
int x,y;

/* Locates the index of the selected opponent */

{
   int z = -1;
   
   do {
      z++;
      } while(((Quest->opponent[z].x != x) ||
       (Quest->opponent[z].y != y) ||
       (Quest->opponent[z].level != Level) ||
       (Quest->opponent[z].type == 0)) && (z < 50));
  return(z);
  }
   
   

Boolean VisibleObject(x,y,e)
int x,y,e;

{
   if(Quest->skill == 0) return(TRUE);
   if((e == 18) || (e == 26) || (e == 38) || (e == 42)
    || (e == 46)) {
	  if(x+y >= 8) return(TRUE);
	  return(FALSE);
	  }
   if((e == 19) || (e == 27) || (e == 36) || (e == 40)
    || (e == 44)) {
	  if(x+y <= 8) return(TRUE);
	  return(FALSE);
	  }
   if((e == 20) || (e == 28) || (e == 39) || (e == 43)
    || (e == 47)) {
	  if(x-y <= 0) return(TRUE);
	  return(FALSE);
	  }
   if((e == 21) || (e == 29) || (e == 37) || (e == 41)
    || (e == 45)) {
	  if(x-y >= 0) return(TRUE);
	  return(FALSE);
	  }
   return(TRUE);
   }
   
   
DrawIt(x,y,x1,y1,buf)
int x,y,x1,y1,buf;

{
   register int z,f,e,i;

   if((x1 >= 0) && (x1 <= 49) && (y1 >= 0) && (y1 <= 49)) {
      e = Quest->map[Level][x1][y1];
      BlitElement(e,x,y,buf,srcCopy);
      z = obs[Level][x1][y1];
	  if((e <= 47) && (!VisibleObject(x,y,e))) z = 0;
      if((z > 0) && (z < 100)) {
	     if((z != 39) || ((e != 84) && (e != 85) && (e != 86)))
		  DrawMasked(z+100,x,y,e,buf);
         }
      else if(z >= 100) {
	    i = FindEnemy(x1,y1);
        f = Quest->opponent[i].facing;
		if((z == 102) && (Quest->opponent[i].bonusmove == 15))
		 DrawCamoed(216+f*2,x,y,e,buf);
		else {
		   z = oppmap[z-100];
		   if(z != 376) DrawMasked((z-4)+f*2,x,y,e,buf);
		   else DrawMasked(376,x,y,e,buf);
		   }
        }
      z = GetMarL(Level,x1,y1);
	  if((e <= 47) && (!VisibleObject(x,y,e))) z = 15;
      if(z != 15) DrawPlayer(z,x,y,e,buf);
	  if(GetMarH(Level,x1,y1) != 0) BlitElement(100,x,y,buf,notSrcBic);
      }
   else BlitElement(70,x,y,buf,srcCopy);
   }



DrawGrid(x,y)
int x,y;

{
   int x1,y1;

   x1 = x * 24;
   y1 = y * 18 + 16;
   SetColor(4);
   if(y == 8) {
      MoveTo(x1+24,y1+18);
      DrawTo(x1,y1+18);
      }
   else MoveTo(x1,y1+18);
   DrawTo(x1,y1);
   DrawTo(x1+24,y1);
   if(x == 8) DrawTo(x1+24,y1+18);
   }
   
   

DrawAllGrid()

{
   int x,y;
  
   SetColor(4);
   for(x = 0; x <= 216; x += 24) {
      MoveTo(x,16);
      DrawTo(x,178);
  	  }
   for(y = 16; y < 196; y += 18) {
      MoveTo(0,y);
      DrawTo(215,y);
      }
   }
  
  
  
DrawSquare(x,y,buff)
int x,y,buff;

{
   register int x1,y1;

   x1 = x + MapOrigin.x;
   y1 = y + MapOrigin.y;
   DrawIt(x,y,x1,y1,buff);
   }


DrawCkSquare(x,y,buff)
int x,y,buff;

{
   register int x1,y1;

   x1 = x + MapOrigin.x;
   y1 = y + MapOrigin.y;
   if((x >= 0) && (y >= 0) && (x <= 8) && (y <= 8)) DrawIt(x,y,x1,y1,buff);
   }


BitSet(l,bt)
Byte *l;
long bt;

{

   *l = *l | pows[bt];
   }



Boolean BitTst(l,bt)
Byte *l;
long bt;

{
   if(*l & pows[bt]) return(TRUE);
   return(FALSE);
   }



DrawAltMap()

/* Draw the map in the ALT buffer */

{
   int x,y,z,f,x1,y1;

   SetSee(3);
   for(x = 0; x <= 8; x++) for(y = 0; y <= 8; y++) {
      if(see[x+4][y+4]) DrawSquare(x,y,1);
      else BlitElement(70,x,y,1,srcCopy);
      if((x == CursorPos.x) && (y == CursorPos.y) && CursorFlag) {
         BlitElement(102,x,y,1,notSrcBic);
         BlitElement(101,x,y,1,srcOr);
  	     }
      }
   }
   
   

Boolean SlashWall(e)
int e;

{
   if((e == 18) || (e == 19) || (e == 26) || (e == 27)) return(TRUE);
   if((e == 22) || (e == 30)) return(TRUE);
   if((e >= 36) && (e <= 50) && ((e & 0x01) == 0)) return(TRUE);
   return(FALSE);
   }
   
   
Boolean BackslashWall(e)
int e;

{
   if((e == 20) || (e == 21) || (e == 28) || (e == 29) || (e == 52)) return(TRUE);
   if((e == 23) || (e == 31)) return(TRUE);
   if((e >= 37) && (e <= 49) && ((e & 0x01) != 0)) return(TRUE);
   return(FALSE);
   }
   
   
SetSee(loops)
int loops;

{
   int x,y,x1,y1,z,dx,dy,dz,ox,oy,q,ring,zmax,ends,low,upp;

   for(x = 0; x < 17; x++) for (y = 0; y < 17; y++) see[x][y] = 0;
   if(GetMarH(Level,MapOrigin.x+4,MapOrigin.y+4) != 0) {
      see[8][8] = 1;
	  return;
	  }
   if(Quest->skill == 0) {
      for(x = 0; x < 17; x++) for (y = 0; y < 17; y++) see[x][y] = 1;
      return;
      }
   for(x = 7; x <= 9; x++) for(y = 7; y <= 9; y++) 
    see[x][y] = viewer[Quest->map[Level][x+MapOrigin.x-4][y+MapOrigin.y-4]];
   if(see[7][8] || see[8][7]) see[7][7] = 1; else see[7][7] = 0;
   if(see[9][8] || see[8][9]) see[9][9] = 1; else see[9][9] = 0;
   if(see[7][8] || see[8][9]) see[7][9] = 1; else see[7][9] = 0;
   if(see[9][8] || see[8][7]) see[9][7] = 1; else see[9][7] = 0;
   z = Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4];
   if((z == 20) || (z == 28) || (z == 39) || (z == 43) || (z == 47)) {
      see[7][9] = 0;
	  see[7][8] = 0;
	  see[8][9] = 0;
	  if(SlashWall(Quest->map[Level][MapOrigin.x+5][MapOrigin.y+5])) 
	   see[9][9] = 0;
	  }
   if((z == 18) || (z == 26) || (z == 38) || (z == 42) || (z == 46)) {
      see[9][9] = 0;
	  see[8][9] = 0;
	  see[9][8] = 0;
	  if(BackslashWall(Quest->map[Level][MapOrigin.x+3][MapOrigin.y+5])) 
	   see[7][9] = 0;
	  }
   for(ring = 0; ring < loops; ring++) {
   zmax = ring*8 + 16;   
   dx = 1;
   dy = 0;
   x = 6-ring;
   y = 6-ring;
   ox = 1;
   oy = 1;
   q = 0;
   x1 = x + MapOrigin.x - 4;
   y1 = y + MapOrigin.y - 4;
   ends = ring*2+4;
   low = 6-ring;
   upp = 10+ring;
   for(z = 0; z < zmax; z++) {
      if(z % ends  == 0) {
         if((see[x+ox][y+oy]) && (viewer[Quest->map[Level][x1+ox][y1+oy]] ==
		  1)) {
            if((viewer[Quest->map[Level][x1][y1+oy]] == 1)
             || (viewer[Quest->map[Level][x1+ox][y1]] == 1))
		     see[x][y] = 1;
			}
         }
      else if(x == 8) {
         if(y < 8) dz = 1;
         else dz = -1;
         if((see[x][y+dz]) && (viewer[Quest->map[Level][x1][y1+dz]])) see[x][y] = 1;
         }
      else if(y == 8) {
         if(x < 8) dz = 1;
         else dz = -1;
         if((see[x+dz][y]) && (viewer[Quest->map[Level][x1+dz][y1]])) see[x][y] = 1;
         }
      else switch(q) {
         case 0:
            if(x < 8) dz = 1;
            else dz = -1;
            if((see[x][y+1]) && (see[x+dz][y+1]) &&
             (viewer[Quest->map[Level][x1][y1+1]]) &&
             (viewer[Quest->map[Level][x1+dz][y1+1]])) see[x][y] = 1;
            break;
         case 1:
            if(y < 8) dz = 1;
            else dz = -1;
            if((see[x-1][y]) && (see[x-1][y+dz]) &&
             (viewer[Quest->map[Level][x1-1][y1]]) &&
             (viewer[Quest->map[Level][x1-1][y1+dz]])) see[x][y] = 1;
            break;
         case 2:
            if(x < 8) dz = 1;
            else dz = -1;
            if((see[x][y-1]) && (see[x+dz][y-1]) &&
             (viewer[Quest->map[Level][x1][y1-1]]) &&
             (viewer[Quest->map[Level][x1+dz][y1-1]])) see[x][y] = 1;
            break;
         case 3:
            if(y < 8) dz = 1;
            else dz = -1;
            if((see[x+1][y]) && (see[x+1][y+dz]) &&
             (viewer[Quest->map[Level][x1+1][y1]]) &&
             (viewer[Quest->map[Level][x1+1][y1+dz]])) see[x][y] = 1;
            break;
         }
      x += dx;
      x1 += dx;
      y += dy;
      y1 += dy;
      if((x == upp) && (y == low)) {
         dx = 0;
         dy = 1;
         ox = -1;
         q = 1;
         }
      if((x == upp) && (y == upp)) {
         dx = -1;
         dy = 0;
         oy = -1;
         q = 2;
         }
      if((x == low) && (y == upp)) {
         dx = 0;
         dy = -1;
         ox = 1;
         q = 3;
         }
      }
   }
   if(see[7][8] || see[8][7]) see[7][7] = 1;
   if(see[9][8] || see[8][9]) see[9][9] = 1;
   if(see[7][8] || see[8][9]) see[7][9] = 1;
   if(see[9][8] || see[8][7]) see[9][7] = 1;
   see[7][8] = 1;
   see[8][7] = 1;
   see[8][9] = 1;
   see[9][8] = 1;
   see[8][8] = 1;
   }



DrawAltSetMap()

{
   int x,y,x1,y1,z;
   int dx,dy,dz,ox,oy;

   SetSee(3);
   for(x = 0; x <= 8; x++) for(y = 0; y <= 8; y++) {
      x1 = x + MapOrigin.x;
      y1 = y + MapOrigin.y;
      if(see[x+4][y+4]) {
         DrawSquare(x,y,1);
         if((CurMarine != 10) && (x1 >= 0) && (x1 < 50) &&
          (y1 >= 0) && (y1 < 50))
          BitSet(&Quest->vis[x1][y1],(long)Level);
         }
      else BlitElement(70,x,y,1,srcCopy);
      if((x == CursorPos.x) && (y == CursorPos.y) && CursorFlag) {
         BlitElement(102,x,y,1,notSrcBic);
         BlitElement(101,x,y,1,srcOr);
  	     }
      }
   LoadPartial();
   }



DrawListIcon()

{
   int x,cm = -1;
   
   for(x = 0; x < 10; x++) {
      if(iconmap[x] == CurMarine) cm = x;
      if((iconmap[x] == 10) || (Quest->marine[iconmap[x]].condition <= 0))
	   BlitMan(x,-10);
	  else BlitMan(x,((Quest->marine[iconmap[x]].condition-1) / 20) + (-9));
	  }
   BlitMan(10,-11);
   SetColor(5);
   cm *= 8;
   if(cm >= 0) Rectangle((long)(cm+233),34L,(long)(cm+240),47L);
   }
   
   
   
DrawMapWindow()

{
   if(MapWindow != NULL) {
      DrawListIcon();
      DrawAltSetMap();
      SaveFlag = FALSE;
      }
   }



ChangeFacing(facing)
int facing;

{
   modifier = FALSE;
   CurMove--;
   Quest->marine[CurMarine].vitality--;
   Quest->marine[CurMarine].facing = facing;
   SavePartial();
   DrawSquare(4,4,1);
   LoadPartial();
   }
   
   

XportEffect(x,y)
int x,y;

{
   int z;
   
   for(z = 0; z < 10; z++) {
      BlitElement(104,x,y,0,notSrcBic);
	  BlitElement(((z & 1) << 1) + 103,x,y,0,srcOr);
	  Delay(10L);
	  }
   }
   
   
   
Boolean CheckTText(x,y,l)
int x,y,l;

{
   int z,tox,toy,tolevel;

   for(z = 0; z < 20; z++) if((Quest->ttext[z].level == l) &&
    (Quest->ttext[z].y == y) && (Quest->ttext[z].x == x) &&
    (!Quest->ttext[z].read)) {
      ShowTText(z);
      }
   /* check for prox charge */
   if(obs[l][x][y] == 39) {
      DrawMapWindow();
      IconMessage(75);
      DetonateProx(x,y);
	  if(Quest->marine[CurMarine].condition <= 0) return(TRUE);
	  }
   /* check for teleport */
   if(Quest->map[l][x][y] == 63) {
      z = -1;
      do {
         z++;
         } while((Quest->tport[z].level != l) || (Quest->tport[z].x != x)
          || (Quest->tport[z].y != y));
      tox = Quest->tport[z].tox;
      toy = Quest->tport[z].toy;
      tolevel = Quest->tport[z].tolevel;
      if((GetMarL(tolevel,tox,toy) != 15) ||
       ((!CheckMove(tolevel,tox,toy)) && (obs[tolevel][tox][toy] < 100)))
       return(FALSE);
	  SoundEff(15);
      SetPt(&MapOrigin,x-4,y-4);
	  DrawMapWindow();
	  XportEffect(4,4);
      SetMarL(Level,x,y,15);
      SetPt(&MapOrigin,tox - 4,toy-4);
      Level = tolevel;
	  DrawMapWindow();
	  XportEffect(4,4);
      MarineCoord(CurMarine,tox,toy,tolevel);
      if(FindEnemy(tox,toy) != numopp) KillNEEnemy(FindEnemy(tox,toy));
      return(TRUE);
      }
   return(FALSE);
   }



MarineCoord(z,x,y,l)
int z,x,y,l;

{
   Quest->marine[z].x = x;
   Quest->marine[z].y = y;
   Quest->marine[z].level = l;
   SetMarL(l,x,y,z);
   CheckTText(x,y,l);
   }



Boolean CheckMove(l,x,y)
int l,x,y;

{
   int e;
   
   if((x < 0) || (x > 49) || (y < 0) || (y > 49)) return(FALSE);
   if(GetMarL(l,x,y) != 15) return(FALSE);
   if(obs[l][x][y] >= 100) return(FALSE);
   e = Quest->map[l][x][y];
   if(passer[e] == 0) return(FALSE);
   if((passer[e] == 2) && (!BeltFlag)) return(FALSE);
   if((BeltFlag) && (CurMove >= 1)) return(TRUE);
   if(CurMove < movc[e]) return(FALSE);
   return(TRUE);
   }
   
   

CheckStun()

{
   if((Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4] == 62) && 
    (Rnd(100) > Quest->marine[CurMarine].vitality)) {
      CurMove = 0;
      Quest->marine[CurMarine].vitality = 0;
      IconMessage(39);
      }
   }
   
   


InvertSquare(x,y)
int x,y;

{
   InvertRect(x*24,y*18 + 16,x*24 + 30,y*18 + 34);
   }



DrawPath(x1,y1,x2,y2,c)
int x1,y1,x2,y2;
long *c;

{
   int dx = 0,dy = 0;
   int q = 0,z = 0;

   x1 = x1 * 24 + 12;
   y1 = y1 * 18 + 25;
   x2 = x2 * 24 + 12;
   y2 = y2 * 18 + 25;
   if(x2 > x1) dx = 1;
   else if (x2 < x1) dx = -1;
   if(y2 > y1) dy = 1;
   else if (y2 < y1) dy = -1;
   if((dx != 0) && (dy != 0)) q = 1;
   SetColor((int)(*c));
   do {
      if(x1 != x2) x1 += dx;
      if(y1 != y2) y1 += dy;
      if((q == 1) && ((z % 4) == 0) && (x1 != x2)) x1 += dx;
      MoveTo(x1,y1);
      DrawTo(x1,y1);
      (*c) -= 8;
      if(*c < 5) *c = 13;
      SetColor((int)(*c));
      z++;
      } while((x1 != x2) || (y1 != y2));
   }


UpdateMoves(cost)
int cost;

{
   char astr[10];

   itoa(CurMove-cost,astr,10);
   strcat(astr,"  ");
   PrintLineXY(astr,280,65);
   }



Boolean CheckPathMove(curelm,dir,cost,diag,l,x,y)
int cost,l,x,y,dir,curelm;
Boolean diag;

{
   int e,mc,e1,e2;
   
   if(see[x+4][y+4] == 0) return(FALSE);
   x += MapOrigin.x;
   y += MapOrigin.y;
   if((x < 0) || (x > 49) || (y < 0) || (y > 49)) return(FALSE);
   if(GetMarL(l,x,y) != 15) return(FALSE);
   if(obs[l][x][y] >= 100) return(FALSE);
   e = Quest->map[l][x][y];
   if(passer[e] == 0) return(FALSE);
   if((passer[e] == 2) && (!BeltFlag)) return(FALSE);
   if(diag) {
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
	   BackslashWall(e2))) return(FALSE);
	  if(((dir == 3) || (dir == 7)) && (SlashWall(e1) ||
	   SlashWall(e2))) return(FALSE);
	  }
   if(BeltFlag) {
      mc = 1;
	  if(diag) mc++;
	  }
   else {
      mc = movc[e];
      if(diag) mc += movc[e] / 2;
	  }
   if(CurMove-cost < mc) return(FALSE);
   if(curelm <= 52) {
      if(wallmoves[curelm-18] & (0x01 << dir)) return(FALSE);
	  }
   return(TRUE);
   }
   
   
   
CheckPathStun()

{
   if((Quest->map[Level][MapOrigin.x+4][MapOrigin.y+4] == 62) && 
    (Rnd(100) > Quest->marine[CurMarine].vitality)) {
      CurMove = 0;
      Quest->marine[CurMarine].vitality = 0;
      IconMessage(39);
      return(TRUE);
      }
   return(FALSE);
   }
   
   


Boolean CheckPathTText(x,y,l)
int x,y,l;

{
   int z,tox,toy,tolevel;

   for(z = 0; z < 20; z++) if((Quest->ttext[z].level == l) &&
    (Quest->ttext[z].y == y) && (Quest->ttext[z].x == x) &&
    (!Quest->ttext[z].read)) {
      DrawMapWindow();
      ShowTText(z);
      }
   /* check for prox charge */
   if(obs[l][x][y] == 39) {
      DrawMapWindow();
      IconMessage(75);
      DetonateProx(x,y);
	  if(Quest->marine[CurMarine].condition <= 0) return(TRUE);
	  }
   /* check for teleport */
   if(Quest->map[l][x][y] == 63) {
      z = -1;
      do {
         z++;
         } while((Quest->tport[z].level != l) || (Quest->tport[z].x != x)
          || (Quest->tport[z].y != y));
      tox = Quest->tport[z].tox;
      toy = Quest->tport[z].toy;
      tolevel = Quest->tport[z].tolevel;
      if((GetMarL(tolevel,tox,toy) != 15) ||
       ((!CheckMove(tolevel,tox,toy)) && (obs[tolevel][tox][toy] < 139)))
       return(FALSE);
      SoundEff(15);
      SetPt(&MapOrigin,x-4,y-4);
	  DrawMapWindow();
	  XportEffect(4,4);
      SetMarL(Level,x,y,15);
      SetPt(&MapOrigin,tox - 4,toy-4);
      Level = tolevel;
	  DrawMapWindow();
	  XportEffect(4,4);
      MarineCoord(CurMarine,tox,toy,tolevel);
      if(FindEnemy(tox,toy) != numopp) KillNEEnemy(FindEnemy(tox,toy));
      return(TRUE);
      }
   return(FALSE);
   }



Boolean PathMove(curelm,tx,ty,path,PathLen,cost,diag,x,y,c)
Byte path[20][2];
int *PathLen,*cost,x,y,tx,ty;
long *c;
Boolean *diag;

{
   int dir;
   
   if(ty < -1) ty = -1;
   else if(ty > 1) ty = 1;
   if(tx < -1) tx = -1;
   else if(tx > 1) tx = 1;
   if(tx < 0) dir = 6 - ty;
   else if(tx > 0) dir = 2 + ty;
   else {
      if(ty < 0) dir = 0;
	  else dir = 4;
	  }
   if(CheckPathMove(curelm,dir,*cost,*diag,Level,x,y)) {
      DrawPath(path[(*PathLen)-1][0],path[(*PathLen)-1][1],x,y,c);
      path[*PathLen][0] = x;
      path[*PathLen][1] = y;
      (*PathLen)++;
	  if(BeltFlag) {
	     (*cost)++;
		 if(*diag) (*cost)++;
		 }
      else {
	     *cost += movc[Quest->map[Level][x+MapOrigin.x][y+MapOrigin.y]];
         if(*diag) *cost += 
          movc[Quest->map[Level][x+MapOrigin.x][y+MapOrigin.y]] / 2;
		 }
      UpdateMoves(*cost);
      return(TRUE);
      }
   return(FALSE);
   }









