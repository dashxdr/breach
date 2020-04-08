/* breach28.c - Copy Protection */

#include   "breach2.h"

/* External Procedures */
extern char *itoa();

/* External Variables */
extern Byte spage[43];
extern Byte sparagraph[43];
extern int scodes[43];



Security()

/* Returns 1 if player passes the security check */

{
   int x,z,tries;
   char astr[40],bstr[50];
   int csum;

   /* Pick a security password */
   x = Rnd(43);
   for (tries = 1; tries <= 3; tries++) {
      SaveBuffer();
	  if(tries != 1) PageFlip(1);
      SetUpDialog();
      PrintLineXY("Please enter word number 1",72,80);
	      strcpy(bstr,"from page ");
      strcat(bstr,itoa(spage[x],astr,10));
      strcat(bstr,", line ");
      strcat(bstr,itoa(sparagraph[x],astr,10));
      strcat(bstr," in");
      PrintLineXY(bstr,72,88);
      PrintLineXY("the Breach 2 manual.",72,96);
      SetColor(31);
      RectangleFill(35,5,285,31);
      SetColor(4);
      Rectangle(35L,5L,285L,31L);
      PrintLineXY("You are entering a FWSF secure data",55,10);
      PrintLineXY("area.  Please enter identification code.",40,18);
      SetColor(31);
      RectangleFill(60,165,260,191);
      PrintLineXY("Count all lines including bold",70,170);
      PrintLineXY("face paragraph titles.",94,180);
      astr[0] = '\000';
	  if(tries != 1) PageFlip(0);
      else FadeIn();
      EditBox(100,109,20,astr);
      csum = 0;
      for(z = 0; z < strlen(astr); z++) {
         astr[z] = toupper(astr[z]);
         csum += (int)astr[z];
         }
      if(csum == scodes[x]) {
	     FadeOut();
         LoadBuffer();
	     return(1);
		 }
	  PageFlip(1);
      LoadBuffer();
	  PageFlip(0);
      if(tries != 3) IconMessage(36);
      else IconMessage(37);
      }
   return(0);
   }

