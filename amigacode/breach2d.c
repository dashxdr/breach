/* breach2d.c - Interrupt service */

#include   "breach2s.h"

int r13 = 0,r5 = 14,d13 = 1,d5 = -1,pulse = 0;
extern struct Screen *MyScreen;
extern struct ViewPort MyViewPort;
extern Boolean PulseFlag,CursorFlag;
extern Point CursorPos;


MyServer()

{
   int_start();
/*   if(PulseFlag) {
      pulse++;
	  if(pulse == 300) {
	     SetColor(1);
		 Rectangle(96L,88L,119L,105L);
		 Rectangle(97L,89L,118L,104L);
		 }
	  else if(pulse == 330) {
	     pulse = 0;
		 DrawSquare(4,4,0);
         if((4 == CursorPos.x) && (4 == CursorPos.y) && CursorFlag) {
            BlitElement(102,4,4,0,notSrcBic);
            BlitElement(101,4,4,0,srcOr);
			}
		 }
      }*/
   r13 += d13;
   if(r13 >= 14) d13 = -1;
   if(r13 <= 0) d13 = 1;
   r5 += d5;
   if(r5 >= 14) d5 = -1;
   if(r5 <= 0) d5 = 1;
   SetRGB4(&MyScreen->ViewPort,5L,(long)r5,(long)r5,0L);
   SetRGB4(&MyScreen->ViewPort,13L,(long)r13,(long)(r13 >> 2),0L);
   SetRGB4(&MyViewPort,5L,(long)r5,(long)r5,0L);
   SetRGB4(&MyViewPort,13L,(long)r13,(long)(r13 >> 2),0L);
   int_end();
   }


