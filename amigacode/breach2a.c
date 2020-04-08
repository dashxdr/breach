/* breach2a.c - crush/uncrush */
#include "breach2.h"

/* External Variables */
extern unsigned char *Quest[];
extern unsigned char crushed[sizeof(struct QstStru)];
extern int freq[256][2];
extern long crulen;
extern int pow[8];
extern long bitptr,bytptr;

unsigned char getbyt()

{
   int b,x;

   b = 0;
   for(x = 0; x < 8; x++) {
       if(crushed[bytptr] & pow[bitptr]) b += pow[x];
       bitptr++;
      if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
       }
   return(b);
   }



uncrushit()

{
   int z,x;
   int maper[7];

   bitptr = 0;
   bytptr = 256;
   for(z = 0; z < 256; z++) if(crushed[z] != 7) {
       maper[crushed[z]] = z;
       }
   for(z = 0; z < sizeof(struct QstStru); z++) {
       x = 0;
       while(((crushed[bytptr] & pow[bitptr]) == 0) && (x < 7)) {
           x++;
       bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
           }
       if(x < 7) {
           *(Quest[0]+z) = maper[x];
       bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
           }
       else *(Quest[0]+z) = getbyt();
       }
   }



int comp(a,b)
int *a,*b;

{
   if( *a < *b) return(1);
   if( *a > *b) return(-1);
   return(0);
   }



addbyte(c)
unsigned char c;

{
   int b,x;

   for(x = 0; x < 8; x++) {
       b = c & pow[x];
       if(b == 0) {
       bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
           }
       else {
         crushed[bytptr] = crushed[bytptr] | pow[bitptr];
         bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
           }
       }
   }



crushit()

{
   int z,x;

   for(z = 0; z < 256; z++) {
       freq[z][1] = z;
       freq[z][0] = 0;
       }
   for(z = 0; z < sizeof(struct QstStru); z++) freq[*(Quest[0]+z)][0]++;
   qsort(freq,256,4,comp);
   for(z = 0; z < 256; z++) {
      x = -1;
       do {
           x++;
           } while(freq[x][1] != z);
       crushed[z] = x;
       if(x > 6) crushed[z] = 7;
       }
   bitptr = 0;
   bytptr = 256;
   for(z = 256; z < sizeof(struct QstStru); z++) crushed[z] = 0;
   for(z = 0; z < sizeof(struct QstStru); z++) {
      for(x = 0; x < crushed[*(Quest[0]+z)]; x++) {
       bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
       }
       if(crushed[*(Quest[0]+z)] < 7) {
         crushed[bytptr] = crushed[bytptr] | pow[bitptr];
         bitptr++;
         if(bitptr > 7) {
          bitptr = 0;
          bytptr++;
          }
           }
       else addbyte(*(Quest[0]+z));
       }
   crulen = bytptr + 257;
   }
