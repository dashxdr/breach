/* breach2b.c - Briefing, Victory */

#include "breach2.h"

/* External Variables */
extern struct QstStru *Quest;
extern Boolean Changed;
extern char *tname[];
extern int Level;
extern Byte DefEncMax[6];
extern Byte DefBonus[6];
extern Point MapOrigin;
extern Point MousePos;

/* External Procedures */
extern Boolean PtInRect();



ShowEditBox()

{
   char buffer[541];
   char astr[2];
   int x,r = 1,c = 0,cnt = 0;
   int x1 = 0, y1 = 0, x2 = 35, y2 = 16;

   if(Quest->BriefLen == 0) return;
   strcpy(astr," ");
   for(x = 0; x < (int)Quest->BriefLen; x++) {
      buffer[x] = Quest->BriefText[x];
      if(buffer[x] == '\000') buffer[x] = ' ';
      }
   buffer[(int)Quest->BriefLen] = '\000';
   do {
       c++;
       if(c > (x2-x1+1)) {
           c = 1;
           r++;
           }
	   astr[0] = buffer[cnt++];
       PrintLineXY(astr,c*6+12-4,r*8+23);
       } while(buffer[cnt] != '\000');
   }



DoBriefing()

{
   FlashButton(BX1+2,BY1+2,BX2-2,BY2-2);
   SaveBuffer();
   SetColor(15);
   RectangleFill(8,15,241,194);
   SetColor(6);
   Rectangle(8L,15L,241L,194L);
   PrintLineXY("Mission Briefing",82,22);
   ShowEditBox();
   Rectangle(100L,179L,150L,190L);
   PrintLineXY("OK",120,182);
   KeyWait();
   FlashButton(100,180,150,189);
   LoadBuffer();
   }



int SatObj(onum)
int onum;

/* Returns TRUE if the object victory condition has been satisfied */

{
   int i = -1;
   do {
      i++;
      if((Quest->object[i].type == onum) &&
       (Quest->object[i].held == FALSE)) return(FALSE);
      } while(i < (numobj-1));
   return(TRUE);
   }
   
   
   
int SatDes(onum)
int onum;

/* Returns TRUE if the destroy victory condition has been satisfied */

{
   int i = -1;
   do {
      i++;
      if(Quest->object[i].type == onum) return(FALSE);
      } while(i < (numobj-1));
   return(TRUE);
   }
   
   
   
int SatOcc()

/* Returns TRUE if the occupy victory condition has been satisfied */

{
   int z = 0,x;
   
   for(x = 0; x < 10; x++) if((Quest->marine[x].class != 0) &&
    (Quest->marine[x].condition > 0) && (Quest->marine[x].x >= 0)) {
      if(Quest->map[Quest->marine[x].level][Quest->marine[x].x]
	   [Quest->marine[x].y] == 93) z++;
	  }
   if(z >= Quest->OccupyNumber) return(TRUE);
   return(FALSE);
   }
   
   
   
int SatKill()

/* Returns TRUE if the kill victory condition has been satisfied */

{
   int i = 0;
   int exist = 0,killed = 0;
   
   do {
      if(Quest->opponent[i].type != 0) {
           exist++;
         if(Quest->opponent[i].health == 0) killed++;
           }
       i++;
      } while(i < numopp);
   if(((killed*100)/exist) < Quest->KillPercent) return(FALSE);
   return(TRUE);
   }
   
   

CheckIt(lin)
int lin;

{
   SetColor(9);
   MoveTo(74,lin*15+65);
   DrawTo(76,lin*15+68);
   DrawTo(82,lin*15+58);
   }
   
   
   
DoStatus()

{
   int lin = 0,x;
   char astr[40],bstr[10];
   
   FlashButton(OX1+2,OY1+2,OX2-2,OY2-2);
   SaveBuffer();
   PageFlip(1);
   
   for(x = 40; x < 160; x += 8) OrderBar(60,x);
   SetColor(8);
   MoveTo(60,39);
   DrawTo(259,39);
   MoveTo(60,160);
   DrawTo(259,160);
   
   PrintLineXY("MISSION ORDERS",118,45);
   for(x = 0; x < 6; x++) if(Quest->VCond[x]) {
      SetColor(12);
      Rectangle(71L,(long)(lin*15+61),83L,(long)(lin*15+70));
      switch(x) {
	     case 0:
		    PrintLineXY("Rescue Prisoners",96,lin*15+63);
			if(SatObj(11)) CheckIt(lin);
			break;
	     case 1:
		    PrintLineXY("Capture DataPacks",96,lin*15+63);
			if(SatObj(9)) CheckIt(lin);
			break;
	     case 2:
		    PrintLineXY("Exit Combat Area",96,lin*15+63);
			break;
	     case 3:
		    strcpy(astr,"Kill ");
            itoa((int)Quest->KillPercent,bstr,10);
			strcat(astr,bstr);
			strcat(astr,"% Opponents");
		    PrintLineXY(astr,96,lin*15+63);
			if(SatKill()) CheckIt(lin);
			break;
	     case 4:
		    PrintLineXY("Destroy DataPacks",96,lin*15+63);
			if(SatDes(9)) CheckIt(lin);
			break;
	     case 5:
		    strcpy(astr,"Hold ");
            itoa((int)Quest->OccupyNumber,bstr,10);
			strcat(astr,bstr);
			strcat(astr," Occup. Squares");
		    PrintLineXY(astr,96,lin*15+63);
			if(SatOcc()) CheckIt(lin);
			break;
		 }
      lin++;
	  }
   PageFlip(0);
   Rectangle(137L,144L,187L,155L);
   PrintLineXY("OK",156,147);
   KeyWait();
   FlashButton(138,145,186,154);
   PageFlip(1);
   LoadBuffer();
   PageFlip(0);
   }


