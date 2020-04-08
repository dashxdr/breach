/* breach20.c - Low Level Routines */

#include "breach2.h"

/* External Procedures */
extern char *strtok();

/* External Variables */
extern Point MousePos;
extern char *messages[];
extern int letterloc[];


PrintLineXY(str,x,y)
char *str;
int x,y;

/* Prints a line of text using BillyFont at screen coords x,y. */

{
   int i = -1,letr;

   do {
      i++;
	  letr = str[i];
	  if((letr < 32) || (letr > 122)) letr = -1;
	  else letr = letterloc[letr-32];
	  if(letr != -1) BlitLetter(letr,(x + (i * 6)),y);
      } while ((*(str + i) != 13) && (*(str + i) != 0)
         && (*(str + i) != '|'));
   }


Print2LineXY(str,x,y)
char *str;
int x,y;

/* Prints a line of text using BillyFont at screen coords x,y. */

{
   int i = -1,letr;

   do {
      i++;
	  letr = str[i];
	  if((letr < 32) || (letr > 122)) letr = -1;
	  else letr = letterloc[letr-32];
	  if(letr != -1) Blit2Letter(letr,(x + (i * 6)),y);
      } while ((*(str + i) != 13) && (*(str + i) != 0)
         && (*(str + i) != '|'));
   }


SetUpDialog()

{
   SetColor(15);
   RectangleFill(50,75,275,125);
   SetColor(0);
   Rectangle(50L,75L,275L,125L);
   SetColor(4);
   Rectangle(51L,76L,274L,124L);
   }



EditBox(x1,y1,wid,buffer)
int x1,y1,wid;
char *buffer;

/* Command line entry routine. Define coordinates in character */
/* position format.                                            */

{
   int c;
   char t[2];
   int count = 0;
   Boolean invflag = FALSE;
   int cwid = 6;
   
   t[1] = 0;
   SetColor(2);
   Rectangle((long)x1 - 4,(long)y1 - 3,(long)x1+(wid*cwid)+3,(long)y1+8);
   do {
      if(count < wid) {
         InvertRect(count*cwid+x1,y1+5,count*cwid+x1+5,y1+5);
         invflag = !invflag;
         }
      c = mygetch();
	  if(c < 0) c = 0;
      if(c > 255) c = 0;
      if((c > 13) && (count < wid)) {
         t[0] = c;
         PrintLineXY(t,count*cwid+x1,y1);
         invflag = FALSE;
         buffer[count] = c;
         count++;
         }
      if((c == 8) && (count > 0)) {
         if(invflag)
          InvertRect(count*cwid+x1,y1+5,count*cwid+x1+5,y1+5);
         invflag = FALSE;
         count--;
         t[0] = 32;
         PrintLineXY(t,count*cwid+x1,y1);
         }
      } while(c != 13);
   buffer[count] = 0;
   }   



EraseGameScreen()

{
   SetColor(0);
   RectangleFill(0,0,319,199);
   }
   
   
   
SetPt(pt,h,v)
Point *pt;
int h,v;

{
   (*pt).x = h;
   (*pt).y = v;
   }



int restrict(x,l,u)
int x,l,u;

{
   if(x > u) return(u);
   if(x < l) return(l);
   return(x);
   }


char *itoa(num,buf,rad)
int num,rad;
char *buf;

{
   char t[7];
   int ptr = 6,x,z;

   z = num;
   for(x = 0; x < 7; x++) t[x] = '\0';
   do {
      x = (num % 10);
      t[--ptr] = (char)(x+48);
      num /= 10;
      } while(num != 0);
   if(z < 0) t[--ptr] = '-';
   strcpy(buf,&t[ptr]);
   return(buf);
   }



KeyWait()

{
   int x;
   
   mygetch();
   do {
      x = mygetch();
	  } while((x == 0) || (x == -3));
   }



IconMessage(id)
int id;

{
   char d[255];
   char *e;
   int row = 91;

   SaveBuffer();
   PageFlip(1);
   strcpy(d,messages[id-1]);
   SetColor(15);
   RectangleFill(50,75,275,125);
   SetColor(0);
   Rectangle(50L,75L,275L,125L);
   SetColor(4);
   Rectangle(51L,76L,274L,124L);
   e = strtok(d,"\n");
   PrintLineXY(e,72,83);
   do {
      e = strtok(NULL,"\n");
      if(e != NULL) PrintLineXY(e,72,row);
      row += 8;
      } while(e != NULL);
   if((id != 48) && (id != 56)) {
      Rectangle(137L,109L,187L,120L);
      PrintLineXY("OK",156,112);
      PageFlip(0);
      KeyWait();
      FlashButton(138,110,186,119);
	  PageFlip(1);
      LoadBuffer();
	  PageFlip(0);
	  }
   else PageFlip(0);
   }



FlashButton(x1,y1,x2,y2)
int x1,y1,x2,y2;

{
   int x;

   for(x = 0; x < 6; x++) {
      InvertRect(x1,y1,x2,y2);
      Delay(2L);
      }
   }



int YesNoBox(id,CancelFlag)
int id;
Boolean CancelFlag;

{
   char d[255];
   char *e;
   int row = 91,sel = 0,c,cu;

   SaveBuffer();
   PageFlip(1);
   strcpy(d,messages[id-1]);
   SetUpDialog();
   SetColor(1);
   Rectangle(67L,109L,117L,120L);
   PrintLineXY("YES",83,112);
   Rectangle(137L,109L,187L,120L);
   PrintLineXY("NO",156,112);
   if(CancelFlag) {
      Rectangle(207L,109L,257L,120L);
      PrintLineXY("CANCEL",216,112);
	  }
   e = strtok(d,"\n");
   PrintLineXY(e,65,83);
   do {
      e = strtok(NULL,"\n");
      if(e != NULL) PrintLineXY(e,65,row);
      row += 8;
      } while(e != NULL);
   PageFlip(0);
   do {
      c = mygetch();
	  if(c > 0) cu = toupper(c);
	  if(cu == 'Y') sel = 1;
	  else if(cu == 'N') sel = 2;
	  else if(CancelFlag && (cu == 'C')) sel = 3;
      else if(c == -1) {
         if(PtInRect(&MousePos,67,109,117,120)) {
            sel = 1;
            FlashButton(68,110,116,119);
            }
         if(PtInRect(&MousePos,137,109,187,120)) {
            sel = 2;
            FlashButton(138,110,186,119);
            }
         if(CancelFlag && PtInRect(&MousePos,207,109,257,120)) {
            sel = 3;
            FlashButton(208,110,256,119);
            }
         }
      } while(sel == 0);
   PageFlip(1);
   LoadBuffer();
   PageFlip(0);
   return(sel);
   }



strptoc(d,s)
char *s,*d;

{
   int len,x;

   len = (int)s[0];
   for(x = 1; x <= len; x++) d[x-1] = s[x];
   d[len] = '\000';
   }



strctop(d,s)
char *s,*d;

{
   int len,x;

   len = strlen(s);
   for(x = len; x > 0; x--) d[x] = s[x-1];
   d[0] = (char)len;
   }



Rectangle(x1,y1,x2,y2)
long x1,y1,x2,y2;

{
   MoveTo((int)x1,(int)y1);
   DrawTo((int)x2,(int)y1);
   DrawTo((int)x2,(int)y2);
   DrawTo((int)x1,(int)y2);
   DrawTo((int)x1,(int)y1);
   }
   
   




