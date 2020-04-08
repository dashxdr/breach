/* breach29.c - System Specifics */

/* Include Files */
#include   "breach2s.h"

/* Externals */
extern char *malloc();
extern MyServer();

/* GIF Variables */
unsigned char holds[4];
int maxcodes[] = {4,8,16,0x20,0x40,0x80,0x100,0x200,0x400,0x800};
int masks[] = {7,15,0x1f,0x3f,0x7f,0xff,0x1ff,0x3ff,0x7ff,0xfff};
int codemask[] = {0,1,3,7,15};
int pow2[] = {1,2,4,8,16,32,64,128,256};
FILE *giffile;
unsigned long bitoffset;
unsigned char rbuf[3],blocksize,*suffix,*outcode,bufmask,finchar;
unsigned int code,curcode,oldcode,bitmask,incode,rcount,outcount;
unsigned int readmask,codesize,eofcode,clearcode,firstfree,freecode,maxcode;
unsigned int initcodesize,*prefix;
unsigned int gifoff,gifline,gifseg;
unsigned char *gifbuf[4];

/* Structures and Defines */
/* External Procedures */
extern char *index();
extern char *rindex();
struct IOExtTD *CreateExtIO();

/* Global Variables */
struct Interrupt *MyInt;
int selup = 0;

struct TextAttr Plop2Font = 
   {
   (STRPTR)"topaz.font",
   8,
   0,
   0,
   };

static struct NewScreen Plop2NewScreen =
   {
   0, 0, 640, 200, 4,
   0, 1,
   HIRES,
   CUSTOMSCREEN | SCREENBEHIND,
   &Plop2Font,
   NULL,
   NULL,
   NULL,
   };

static struct NewWindow My2NewWindow =
   {
   0, 0, 640, 200,
   -1, -1,
   MOUSEBUTTONS | RAWKEY,
   BACKDROP | BORDERLESS | ACTIVATE | RMBTRAP,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   0,0,0,0,
   CUSTOMSCREEN
   };
struct FileLock *ridlock = NULL,*orglock = NULL;
int skeymap[] = {0,33,00,00,00,37,00,00,00,40,41,95,0,0,0,0,81,87,69,82,
                84,89,85,73,79,80,0,0,0,2006,0,2005,65,83,68,70,71,72,74,75,
                76,58,34,0,0,0,2008,0,0,90,88,67,86,66,78,77,0,0,63,0,0,2007,0,
                2004,32,8,9,0,13};
int keymap[] = {0,49,50,51,52,53,54,55,56,57,48,3000,3001,0,0,0,81,87,69,82,
                84,89,85,73,79,80,0,0,0,2006,0,2005,65,83,68,70,71,72,74,75,
                76,59,39,0,0,0,2008,0,0,90,88,67,86,66,78,77,44,46,47,0,0,2007,0,
                2004,32,8,9,0,13};
int keymax = 0x44;
int *MyPointer;
struct Screen *MyScreen,*My2Screen;
struct Window *MyWindow;
struct Window *TempWindow;
struct RastPort *MyRast;
struct Library *GfxBase = NULL;
struct Library *LayersBase = NULL;
struct Library *IntuitionBase = NULL;
struct Library *IconBase = NULL;
struct View MyView;
struct cprlist *MainC1,*MainC2,*AltC1,*AltC2;
struct BitMap *AltBitMap,*MainBitMap;
struct RasInfo MyRasInfo;
struct RastPort MyRastPort;
struct ViewPort MyViewPort;
char tokbuf[150],token[150];
char tfname[20];
char *tokptr;
int elmx[] = {
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288,
 0,24,48,72,96,120,144,168,192,216,240,264,288};
int elmy[] = {
 0,0,0,0,0,0,0,0,0,0,0,0,0,
 18,18,18,18,18,18,18,18,18,18,18,18,18,
 36,36,36,36,36,36,36,36,36,36,36,36,36,
 54,54,54,54,54,54,54,54,54,54,54,54,54,
 72,72,72,72,72,72,72,72,72,72,72,72,72,
 90,90,90,90,90,90,90,90,90,90,90,90,90,
 108,108,108,108,108,108,108,108,108,108,108,108,108,
 126,126,126,126,126,126,126,126,126,126,126,126,126,
 144,144,144,144,144,144,144,144,144,144,144,144,144,
 162,162,162,162,162,162,162,162,162,162,162,162,162,
 180,180,180,180,180,180,180,180,180,180,180,180,180};

struct TextAttr PlopFont = 
   {
   (STRPTR)"topaz.font",  /* I think this is the ROM font */
   8,
   0,
   0,
   };
struct NewScreen PlopNewScreen =
   {
   0, 0, 320, 200, 4,
   0, 1,
   NULL,
   CUSTOMSCREEN|SCREENBEHIND,
   &PlopFont,
   (STRPTR)"Breach 2",
   NULL,
   NULL,
   };
struct NewWindow MyNewWindow =
   {
   0, 0, 320, 200,
   -1, -1,
   RAWKEY | MOUSEMOVE | MOUSEBUTTONS,
   BACKDROP | BORDERLESS | ACTIVATE | RMBTRAP,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   0,0,0,0,
   CUSTOMSCREEN
   };
int pntrZZZ[] = {
    0x0000, 0x0000, 0x0600, 0x0600, 0x0f40, 0x0f40, 0x3fe0, 0x3fe0,
    0x7fe0, 0x7fe0, 0x61f0, 0x7ff0, 0x7bf8, 0x7ff8, 0xf7f8, 0xfff8,
    0x61fc, 0x7ffc, 0x7f0c, 0x7ffc, 0x3fde, 0x3ffe, 0x7fbc, 0x7ffc,
    0x3f0c, 0x3ffc, 0x1ff8, 0x1ff8, 0x07f0, 0x07f0, 0x01c0, 0x01c0,
    0x0700, 0x0700, 0x0fc0, 0x0fc0, 0x0680, 0x0680, 0x0000, 0x0000,
    0x00c0, 0x00c0, 0x00e0, 0x00e0, 0x0040, 0x0040, 0x0000, 0x0000
 };
USHORT pntrNoE[]=  {
     /*plane1  plane0 */
      0x0000,  0x0000,
      0x0000,  0x0000,
      0x0000,  0x0ff0,
      0x0ff0,  0x300c,
      0x1ffc,  0x6fe2,
      0x3ffc,  0x5fc2,
      0x7ffe,  0xbf8d,
      0x7ffe,  0xbf1d,
      0x7ffe,  0xbe3d,
      0x7ffe,  0xbc7d,
      0x7ffe,  0xb8fd,
      0x7ffe,  0xb1fd,
      0x3ffc,  0x43fa,
      0x3ff8,  0x47f6,
      0x0ff0,  0x300c,
      0x0000,  0x0ff0,
      0x0000,  0x0000,
      0x0000,  0x0000
     };

USHORT pntrKey[]=  {
     /*plane1  plane0 */
      0x0000,  0x0000,
      0x1800,  0x0000,
      0x2400,  0x1800,
      0x5bfe,  0x2400,
      0xa402,  0x43fc,
      0xa402,  0x43fc,
      0x5baa,  0x2454,
      0x24ba,  0x1844,
      0x18ee,  0x0000,
      0x0000,  0x0000,
     };
USHORT pntrCross[]=  {
     /*plane1  plane0 */
      0x0000,  0x0000,
      0x07c0,  0x0000,
      0x07c0,  0x0380,
      0x07c0,  0x0100,
      0x0380,  0x0100,
      0x0380,  0x0100,
      0xe7ce,  0x0100,
      0xfefe,  0x4384,
      0xfc7e,  0x7efc,
      0xfefe,  0x4384,
      0xe7ce,  0x0100,
      0x0380,  0x0100,
      0x0380,  0x0100,
      0x07c0,  0x0100,
      0x07c0,  0x0380,
      0x07c0,  0x0000,
      0x0000,  0x0000,
     };
USHORT pntrHand[]=  {
     /*plane1  plane0 */
      0x0000,  0x0000,
      0x0380,  0x0000,
      0x0440,  0x0380,
      0x1b40,  0x0780,
      0x2680,  0x1f00,
      0x5dfe,  0x3e00,
      0x9c01,  0x7ffe,
      0xb9fd,  0x7ffe,
      0xb401,  0x7ffe,
      0x8dee,  0x7ff0,
      0xbc10,  0x7fe0,
      0xbdd0,  0x7fe0,
      0x9c20,  0x7fc0,
      0x4040,  0x3f80,
      0x3f80,  0x0000,
      0x0000,  0x0000
     };
unsigned int rgbmap0[16],rgbmap1[16],rgbmap2[16],rgbmapt[3];
struct BitMap buf2,*screen;
struct BitMap bit[5];
int IntFlag = FALSE;
int letterx[] = {0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,
                 144,152,160,168,176,184,192,200,208,216,224,232,240,248,
                 256,264,272,280,288,296,304,
                 0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,
                 144,152,160,168,176,184,192,200,208,216,224,232,240,248,
                 256,264,272,280,288,296,304};

/* External Variables */
extern Boolean PulseFlag;
extern unsigned char crushed[];
extern long crulen;
extern int passc;
extern char passv[];
extern Boolean NewFlag,Changed,CursorFlag,Invis[],GridFlag,SoundFlag;
extern Byte maskinfo[];
extern struct PalStru Paladin;
extern Point MousePos;
extern char *messages[];
extern struct QstStru *Quest;
extern char SqlInfo[700],GamInfo[700],SceInfo[700];
extern int SqlLen,GamLen,SceLen;
extern BYTE *sndbuf[19],*sndmain;
extern long  sndlen[19],sndmax;
extern int  sndrat[19];
extern BYTE sndpri[];
extern int  sndnum,sndload;
extern int CurMarine,CurMove,Level;
extern Byte movc[];
extern Point MapOrigin,MousePos;
extern Byte mar[5][50][50];
extern Boolean modifier,optioned,BeltFlag;
extern int selector[3],counts[3];
extern char scennames[38][22],leadnames[4][20],leadgame[4][20],gamenames[4][20];
extern char gamescen[4][21];
extern char scenfile[38][13],leadfile[4][13],gamefile[4][13],gamelead[4][13];
extern char fpath[80];
extern Byte scendifs[38];
extern char *diffname[4];
extern int sstart;



#asm
       public    _Rnd

_Rnd   lea rndseed,a0      ;Get address of seed
       move.w  4(sp),d1    ;Get range argument
       tst.w   d1
       ble.s   setseed     ;Go reset seed

       move.l  (a0),d0     ;Get seed
       ADD.L   D0,D0
       BHI.S   over
       EORI.L  #$1D872B41,D0
over
       move.l  d0,(a0)     ;Save new seed
       andi.l  #$ffff,d0   ;Coerce into word
       divu    d1,d0       ;Divide by range
       swap    d0          ; and get remainder (modulus)
       rts

setseed neg.w   d1         ;Probably don't need this
       move.l  d1,(a0)
       rts

       dseg
rndseed        dc.l    0
       cseg
#endasm



InvertRect(x1,y1,x2,y2)
int x1,y1,x2,y2;

{
   BltBitMap(screen,(long)x1,(long)y1,screen,
    (long)x1,(long)y1,(long)x2-x1+1,(long)y2-y1+1,
    0x50L,(long)0xFF,NULL);
   }



mygetch()

{
   long class,code;
   struct IntuiMessage *message;

   if((message=(struct IntuiMessage *) GetMsg(MyWindow->UserPort)) == 0L) 
    return(0);
   class  = message->Class;
   code = message->Code;
   MousePos.x = message->MouseX;
   MousePos.y = message->MouseY;
   if(message->Qualifier & 0x0003L) modifier = TRUE;
   else modifier = FALSE;
   if(message->Qualifier & 0x0010L) optioned = TRUE;
   else optioned = FALSE;
   ReplyMsg(message);
   if(class == MOUSEMOVE) return(-4);
   if(class == MOUSEBUTTONS) {
      if(code == SELECTDOWN) return(-1);
      if(code == MENUDOWN) return(-2);
	  if(code == SELECTUP) return(-3);
      }
   if((class == RAWKEY) && (code < 129)) {
      if(code == 0x3E) code = 0x4C;
      if(code == 0x2F) code = 0x4E;
      if(code == 0x1E) code = 0x4D;
      if(code == 0x2D) code = 0x4F;
      if(code > keymax) {
	     if((code >= 0x50) && (code <= 0x59)) return(1000+code-0x50);
		 if(code == 0x4C) return(2000);
		 if(code == 0x4E) return(2001);
		 if(code == 0x4D) return(2002);
		 if(code == 0x4F) return(2003);
		 if(code == 0x4A) return(3000);
		 if(code == 0x5E) return(3001);
		 return(4000);
		 }
      else {
         if(modifier) code = skeymap[code];
         else code = keymap[code];
         }
      return(code);
      }
   return(0);
   }



ReadDataFile()

{
   FILE *file;
   long loc[11];
   int i;

   file = fopen("Breach2.dat","r");
   for(i = 0; i < 11; i++) fread(&loc[i],1,sizeof(long),file);

   for(i = 0; i < 7; i++) {
      if(i < 3) DecodeGif(file,0);
	  else DecodeGif(file,i-2);
      fseek(file,loc[i+1]+44L,0);
      if((i < 3) && (passc == 0)) ShowTitlePage(i);
      }
   fseek(file,loc[7]+44L,0);
   SceLen = (int)(loc[8] - loc[7]);
   fread(SceInfo,SceLen,1,file);
   fseek(file,loc[8]+44L,0);
   SqlLen = (int)(loc[9] - loc[8]);
   fread(SqlInfo,SqlLen,1,file);
   fseek(file,loc[9]+44L,0);
   GamLen = (int)(loc[10] - loc[9]);
   fread(GamInfo,GamLen,1,file);
   fseek(file,loc[10]+44L,0);
 
   fread(&sndnum,sizeof(sndnum),1,file);
   for(i = 0; i < sndnum; i++) {
      fread(&sndrat[i],2,1,file);
      fread(&sndlen[i],4,1,file);
	  if(sndpri[i]) {
         sndbuf[i] = (BYTE *)AllocMem((long)sndlen[i],(long)MEMF_CHIP | MEMF_CLEAR);
	     }
	  else {
	     if(sndlen[i] > sndmax) sndmax = sndlen[i];
         sndbuf[i] = (BYTE *)AllocMem((long)sndlen[i],(long)MEMF_PUBLIC | MEMF_CLEAR);
         }
      fread(sndbuf[i],1,(int)sndlen[i],file);
      }
   sndmain = (BYTE *)AllocMem(sndmax,(long)MEMF_CHIP | MEMF_CLEAR);
   fclose(file);
   }



RectangleFill(x1,y1,x2,y2)
int x1,y1,x2,y2;

{
   RectFill(MyRast,(long)x1,(long)y1,(long)x2,(long)y2);
   }
   
   
   
SetBColor(x)
int x;

{
   SetBPen(MyRast,(long)x);
   }



SetColor(x)
int x;

{
   if(selup && (x == 10)) x = 1;
   SetAPen(MyRast,(long)x);
   }



BlitLetter(letter,dx,dy)
int letter,dx,dy;

/* Blit will transfer a letter to any location on any buffer. */

{
   long ly;

   if(My2Screen == NULL) { 
      if(letter > 38) ly = 194;
      else ly = 188;
      BltBitMap(&bit[3],(long)letterx[letter],ly,screen,(long)dx,(long)dy,6L,6L,
       (long)COPY_MINTERM,(long)0xFF,NULL); 
	  }
   else {
      if(letter > 38) ly = 6;
      else ly = 0;
      BltBitMap(&bit[4],(long)letterx[letter],ly,screen,(long)dx,(long)dy,6L,6L,
       (long)COPY_MINTERM,(long)0xFF,NULL);  
	  }
   }


Blit2Letter(letter,dx,dy)
int letter,dx,dy;

/* Blit will transfer a letter to any location on any buffer. */

{
   long ly;

   if(letter > 38) ly = 6;
   else ly = 0;
   BltBitMap(&bit[4],(long)(letterx[letter]+320),ly,screen,(long)dx,(long)dy,6L,6L,
    (long)COPY_MINTERM,(long)0xFF,NULL);  
   }


MoveTo(x,y)
int x,y;

{
   Move(MyRast,(long)x,(long)y);
   }



DrawTo(x,y)
int x,y;

{
   Draw(MyRast,(long)x,(long)y);
   }



InitCursor()

{
   ShapeCursor(0);
   }



ShapeCursor(x)
int x;

{
   SetCursor(x,MyWindow);
   }
   
   

SetCursor(cnum,window)
int cnum;
struct Window *window;

{
   int x;
   
   switch(cnum) {
      case 0: ClearPointer(window);
	          for(x = 0; x < 3; x++) SetColors((long)(x+17),
			   (long)((rgbmapt[x] >> 8) & 15),
			   (long)((rgbmapt[x] >> 4) & 15),(long)(rgbmapt[x] & 15));
              break;
      case 1: for(x = 0; x < 48; x++) MyPointer[x] = pntrZZZ[x];
	          SetPointer(window,(struct SpriteImage *)MyPointer,22L,9L,0L,0L);
	          for(x = 0; x < 3; x++) SetColors((long)(x+17),
			   (long)((rgbmapt[x] >> 8) & 15),
			   (long)((rgbmapt[x] >> 4) & 15),(long)(rgbmapt[x] & 15));
              break;
      case 2: for(x = 0; x < 36; x++) MyPointer[x] = pntrNoE[x];
	          SetPointer(window,(struct SpriteImage *)MyPointer,16L,16L,-7L,-7L);
              SetColors(17L,15L,15L,15L);
              SetColors(18L,0L,0L,0L);
              SetColors(19L,15L,0L,0L);
              break;
      case 3: for(x = 0; x < 20; x++) MyPointer[x] = pntrKey[x];
	          SetPointer(window,(struct SpriteImage *)MyPointer,8L,16L,-7L,-7L);
              SetColors(17L,15L,15L,15L);
              SetColors(18L,0L,0L,0L);
              SetColors(19L,14L,3L,0L);
              break;
      case 4: for(x = 0; x < 34; x++) MyPointer[x] = pntrCross[x];
	          SetPointer(window,(struct SpriteImage *)MyPointer,15L,16L,-6L,-6L);
              SetColors(17L,15L,15L,15L);
              SetColors(18L,0L,15L,0L);
              SetColors(19L,15L,3L,0L);
              break;
      case 5: for(x = 0; x < 36; x++) MyPointer[x] = pntrHand[x];
	          SetPointer(window,(struct SpriteImage *)MyPointer,15L,16L,-14L,-5L);
              SetColors(17L,15L,15L,15L);
              SetColors(18L,0L,0L,0L);
              SetColors(19L,14L,9L,8L);
              break;
      }
   }
   


SaveBuffer()

{
    BltBitMap(screen,0L,0L,&buf2,
    0L,0L,320L,200L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



Save2Buffer()

{
    BltBitMap(screen,0L,12L,&bit[4],
    0L,12L,640L,188L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



LoadBuffer()

{
    BltBitMap(&buf2,0L,0L,screen,
    0L,0L,320L,200L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



Load2Buffer()

{
    BltBitMap(&bit[4],0L,12L,screen,
    0L,12L,640L,188L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



copybuffer(src,des)
struct BitMap *src,*des;

{
   if(My2Screen == NULL)
    BltBitMap(src,0L,0L,des,
    0L,0L,320L,200L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   else
    BltBitMap(src,0L,12L,des,
    0L,12L,640L,188L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



/*movebuffer(src,des)
struct BitMap *src,*des;

{
   BltBitMap(src,6L,16L,des,
    6L,16L,189L,153L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }*/



LoadPartial()

{
   BltBitMap(&buf2,0L,16L,screen,
    0L,16L,216L,162L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



SavePartial()

{
   BltBitMap(screen,0L,16L,&buf2,
    0L,16L,216L,162L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



fade_colors(vp,nColors) 
struct ViewPort *vp;
LONG nColors;

{

   WORD col[32];
   long i,more;

   for (i=0;i<nColors;i++)
	col[i] = GetRGB4(vp->ColorMap,i);
   more = TRUE;
   while (more == TRUE) {
      more = FALSE;
	  for (i=0;i<nColors;i++) {
         if ((col[i] & 0x0f00) > 0) { col[i] -= 0x0100; more = TRUE; }
	     if ((col[i] & 0x00f0) > 0) { col[i] -= 0x0010; more = TRUE; }
	     if ((col[i] & 0x000f) > 0) { col[i] -= 0x0001; more = TRUE; }
	     }
      LoadRGB4(vp, &col[0], nColors);
	  Delay(2L);
      }
   }



restore_colors(vp,colortable,nColors) 
struct ViewPort *vp;
LONG nColors;
WORD *colortable;
{

   WORD col[32];
   long i,more;

   for (i=0;i<nColors;i++)
	col[i] = GetRGB4(vp->ColorMap,i);
   more = TRUE;
   while (more == TRUE) {
      more = FALSE;
	
	  for (i=0;i<nColors;i++) {
         if ((col[i] & 0x0f00) < (*(colortable + i) & 0x0f00 ))
		  { col[i] += 0x0100; more = TRUE; }
	     if ((col[i] & 0x00f0) < (*(colortable + i) & 0x00f0))
		  { col[i] += 0x0010; more = TRUE; }
	     if ((col[i] & 0x000f) < (*(colortable + i) & 0x000f))
		  { col[i] += 0x0001; more = TRUE; }
	     if ((col[i] & 0x0f00) > (*(colortable + i) & 0x0f00 ))
		  { col[i] -= 0x0100; more = TRUE; }
	     if ((col[i] & 0x00f0) > (*(colortable + i) & 0x00f0))
		  { col[i] -= 0x0010; more = TRUE; }
	     if ((col[i] & 0x000f) > (*(colortable + i) & 0x000f))
		  { col[i] -= 0x0001; more = TRUE; }
	     }
	  LoadRGB4(vp, &col[0], nColors);
	  Delay(2L);
      }
   }



DoCircle(x,y,r)
int x,y,r;

{
   DrawEllipse(MyRast,(long)x,(long)y,(long)r,(long)r);
   }
   
   
   
char *GetCD()

{
   struct Process *ptr;
   struct FileInfoBlock *FIB;
   struct Lock *cdlock,*prevlock;
   static char newname[100];
   char oldname[100];

   ptr = (struct Process *) FindTask(0L);   /* Get address of current task */
      /* ptr now points to "AmigaDOS Process values",
              see pg 3-1 tech ref, dosextens.h */
   FIB = (struct FileInfoBlock *) AllocMem((long)sizeof(struct
   FileInfoBlock),MEMF_PUBLIC);
   cdlock = (struct Lock *) DupLock((ptr->pr_CurrentDir) /* << 2 */ );
      /* NOTE: Book says it's BPTR, but GURU when shifted, not GURU
         when not shifted.  Explain, please? */
   newname[0] = oldname[0] = '\0';
   do {
      cdlock = (struct Lock *) ParentDir(prevlock = cdlock);
      Examine(prevlock,FIB);
      strcpy(newname, FIB->fib_FileName);
      UnLock(prevlock);
      if (cdlock == 0) {
         /* prevlock was the root of a disk, and thus the name */
         strcat(newname,":");
         strcat(newname,oldname);
         }
      else {
         strcpy(newname,FIB->fib_FileName);
         if (oldname[0]) {
            strcat(newname,"/");
            strcat(newname,oldname);
            }
         strcpy(oldname,newname);
         }
      } while (cdlock != 0);
   FreeMem(FIB,(long)sizeof(struct FileInfoBlock));
   return(newname);
   }



char *strtok(mystr,lookfor)
char *mystr,*lookfor;

{
   char *t;

   if(mystr != NULL) {
      strcpy(tokbuf,mystr);
      tokptr = &tokbuf[0];
      }
   else if(tokptr == NULL) return(NULL);
   t = index(tokptr,lookfor[0]);
   if(t != NULL) t[0] = 0;
   strcpy(token,tokptr);
   if(t != NULL) tokptr = &t[1];
   else tokptr = NULL;
   return(token);
   }



FadeIn()

{
   restore_colors(&MyScreen->ViewPort,rgbmap1,16L);
   }



FadeOut()

{
   fade_colors(&MyScreen->ViewPort,16L);
   }



CloseEm()

{
   struct Screen *WbScreen;
   int rgbmapw[4],i;
   
   WbScreen = (struct Screen *)AllocMem((long)sizeof(struct Screen),
	MEMF_PUBLIC|MEMF_CLEAR);
   GetScreenData(WbScreen,(long)sizeof(struct Screen),WBENCHSCREEN, NULL);
   for(i = 0; i < 4; i++) rgbmapw[i] = GetRGB4((&WbScreen->ViewPort)->ColorMap,(long)i);
   fade_colors(&WbScreen->ViewPort,4L);
   FreeMem(WbScreen,(long)sizeof(struct Screen));

   if (MyWindow != NULL) CloseWindow(MyWindow);
   if (MyScreen != NULL) CloseScreen(MyScreen);

   WBenchToFront();
   WbScreen = (struct Screen *)AllocMem((long)sizeof(struct Screen),
	MEMF_PUBLIC|MEMF_CLEAR);
   GetScreenData(WbScreen,(long)sizeof(struct Screen),WBENCHSCREEN, NULL);
   restore_colors(&WbScreen->ViewPort,rgbmapw,4L);
   FreeMem(WbScreen,(long)sizeof(struct Screen));

   Disable();
   if (IconBase != NULL) CloseLibrary(IconBase);
   if (IntuitionBase != NULL) CloseLibrary(IntuitionBase);
   if (LayersBase != NULL) CloseLibrary(LayersBase);
   if (GfxBase != NULL) CloseLibrary(GfxBase);
   Enable();
   
   }



Terminate()

{
   int i,j;

   if(orglock != NULL) CurrentDir(orglock);
   if(ridlock != NULL) UnLock(ridlock);
   FreeVPortCopLists(&MyViewPort);
   FreeCprList(AltC1);
   FreeCprList(AltC2);
   if(IntFlag) RemIntServer(INTB_VERTB,MyInt);
   FreeSong();
   for(i = 0; i < sndnum; i++) FreeMem(sndbuf[i],(long)sndlen[i]);
   FreeMem(sndmain,sndmax);
   FreeMem(MyPointer,100L);
   FreeMem(MyInt,(long)(sizeof(struct Interrupt)));
   FreeMem(Quest,(long)sizeof(*Quest));
   /* Free bit bitmap space */
   for(i = 0; i < 4; i++) FreeRaster(buf2.Planes[i],320L,200L); 
   for(j = 1; j < 4; j++) 
    for(i = 0; i < 4; i++) FreeRaster(bit[j].Planes[i],320L,200L); 
   for(i = 0; i < 4; i++) FreeRaster(bit[4].Planes[i],640L,200L); 
   CloseEm();   
   }



BlitMan(x,t)
int x,t;

{
   BltBitMap(&bit[1],312L,(long)(((-t)-1)*18),screen,(long)(232+x*8),32L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   }
   
   
   
DrawIconBar()

{
   int x;
   
   /* Place icons on screen */
   BlitAnimate(0,0,180,0,COPY_MINTERM);
   BlitAnimate(1,24,180,0,COPY_MINTERM);
   BlitAnimate(2,72,180,0,COPY_MINTERM);
   BlitAnimate(3,96,180,0,COPY_MINTERM);
   BlitAnimate(4,269,180,0,COPY_MINTERM);
   BlitAnimate(5,293,180,0,COPY_MINTERM);
   BlitAnimate(6,48,180,0,COPY_MINTERM);
   BltBitMap(&bit[1],312L,0L,screen,224L,147L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   BltBitMap(&bit[1],312L,18L,screen,232L,147L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   BltBitMap(&bit[1],312L,36L,screen,240L,147L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   BltBitMap(&bit[1],312L,54L,screen,224L,32L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   for(x = 232; x <= 304; x += 8) 
    BltBitMap(&bit[1],312L,162L,screen,(long)x,32L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   BltBitMap(&bit[1],312L,180L,screen,312L,32L,8L,18L,(long)COPY_MINTERM,(long)0xFF,NULL);
   BlitAnimate(8,203,180,0,COPY_MINTERM);
   BlitAnimate(9,227,180,0,COPY_MINTERM);
   BlitAnimate(10,268,147,0,COPY_MINTERM);
   BlitAnimate(11,292,147,0,COPY_MINTERM);
   BlitAnimate(12,138,180,0,COPY_MINTERM);
   BlitAnimate(13,162,180,0,COPY_MINTERM);
   BlitAnimate(14,270,12,0,COPY_MINTERM);
   BlitAnimate(15,222,12,0,COPY_MINTERM);
   BlitAnimate(16,246,12,0,COPY_MINTERM);
   BlitAnimate(17,294,12,0,COPY_MINTERM);
   }
   
   

DrawGameScreen()

{
   restore_colors(&MyScreen->ViewPort,rgbmap1,16L);
   /* Place icons on screen */
   DrawIconBar();
   /* Draw the time window */
   SetColor(15);
   RectangleFill(221,0,319,9);
   SetColor(6);
   Rectangle(221L,0L,319L,9L);
   PrintLineXY("Time: 00:00:00",228,2);
   /* Draw Name Window */
   SetColor(15);
   RectangleFill(0,0,215,9);
   SetColor(6);
   Rectangle(0L,0L,215L,9L);
   restore_colors(&MyScreen->ViewPort,rgbmap1,16L);
   }
   
   

int Initialize()

{
   int i,j;
   long MemC,MemF;
   struct Screen *WbScreen;
   int rgbmapw[4];

   /* Open libraries */
   if ((GfxBase = OpenLibrary("graphics.library", (long)0)) == NULL)
    exit(-1);
   if ((LayersBase =  OpenLibrary("layers.library", (long)0)) == NULL) {
      CloseEm();
      exit(-2);
      }
   if ((IntuitionBase =  OpenLibrary("intuition.library",(long) 0)) == NULL) {
      CloseEm();
      exit(-3);
      }
   if ((IconBase =  OpenLibrary("icon.library",(long) 0)) == NULL) {
      CloseEm();
      exit(-4);
      }

   WbScreen = (struct Screen *)AllocMem((long)sizeof(struct Screen),
	(long)(MEMF_PUBLIC|MEMF_CLEAR));
   GetScreenData(WbScreen,(long)sizeof(struct Screen),WBENCHSCREEN, NULL);
   for(i = 0; i < 4; i++) rgbmapw[i] = GetRGB4((&WbScreen->ViewPort)->ColorMap,(long)i);

   MyScreen = OpenScreen(&PlopNewScreen);
   ShowTitle(MyScreen,0L);
   MyNewWindow.Screen = MyScreen;
   MyWindow = OpenWindow(&MyNewWindow);
   MyRast = MyWindow->RPort;

   fade_colors(&WbScreen->ViewPort,4L);
   fade_colors(&MyScreen->ViewPort,16L);
   ScreenToFront(MyScreen);
   restore_colors(&WbScreen->ViewPort,rgbmapw,4L);
   FreeMem(WbScreen,(long)sizeof(struct Screen));
   for(i = 0; i < 3; i++) 
    rgbmapt[i] = GetRGB4((&MyScreen->ViewPort)->ColorMap,(long)i+17);
   
   MemF = AvailMem((long)MEMF_FAST);
   MemC = AvailMem((long)MEMF_CHIP);

   if(((MemF+MemC) < 345000) || (MemC < 270000)) {
      CloseEm();
      exit(-5000);
      }

   Rnd((short) -VBeamPos() - 10);  /*  Plant a seed  */
   i = VBeamPos();
   for(j = 0; j < i; j++) Rnd(100);

   /* Allocate the bit bitmaps */
   InitBitMap(&buf2,4L,320L,200L);
   for(i = 0; i < 4; i++) {
      buf2.Planes[i] = (PLANEPTR)AllocRaster(320L,200L);
      }

   InitBitMap(&bit[0],4L,320L,200L);
   for(i = 0; i < 4; i++) bit[0].Planes[i] = buf2.Planes[i];
   
   for(j = 1; j < 4; j++) {
      InitBitMap(&bit[j],4L,320L,200L);
      for(i = 0; i < 4; i++) bit[j].Planes[i] = (PLANEPTR)AllocRaster(320L,200L);
      }
   InitBitMap(&bit[4],4L,640L,200L);
   for(i = 0; i < 4; i++) bit[4].Planes[i] = (PLANEPTR)AllocRaster(640L,200L);
   
   MyPointer = AllocMem(100L,MEMF_CHIP);
   MyInt = AllocMem((long)(sizeof(struct Interrupt)),MEMF_PUBLIC);
   screen = &MyScreen->BitMap;

   ShapeCursor(1);
   ReadDataFile();

   /* Set up double-buffered stuff */
   AltBitMap = &buf2;
   MainBitMap = &MyScreen->BitMap;

   MyRasInfo.BitMap = AltBitMap;
   MyRasInfo.RxOffset = 0;
   MyRasInfo.RyOffset = 0;
   MyRasInfo.Next = NULL;
 
   InitRastPort(&MyRastPort);
   MyRastPort.BitMap = AltBitMap;

   InitVPort(&MyViewPort);
   MyViewPort.RasInfo = &MyRasInfo;
   MyViewPort.DWidth = 320;
   MyViewPort.DHeight = 200;
   MyViewPort.DxOffset = 0;
   MyViewPort.DyOffset = 0;
   MyViewPort.Next = NULL;

   InitView(&MyView);
   MyView.ViewPort = &MyViewPort;
   MyView.DxOffset = ViewAddress()->DxOffset;
   MyView.DyOffset = ViewAddress()->DyOffset;
   MakeVPort(&MyView,&MyViewPort);
   LoadRGB4(&MyViewPort,rgbmap1,16L);
   MrgCop(&MyView);

   AltC1 = MyView.LOFCprList;
   AltC2 = MyView.SHFCprList;

   MyInt->is_Node.ln_Type = NT_INTERRUPT;
   MyInt->is_Node.ln_Pri = -60;
   MyInt->is_Node.ln_Name = "Breach 2";
   MyInt->is_Data = NULL;
   MyInt->is_Code = MyServer;

   fade_colors(&MyScreen->ViewPort,16L);
   ShapeCursor(0);
   CursorFlag = FALSE;
   EraseGameScreen();
   LoadRGB4(&MyViewPort,rgbmap1,16L);
   Quest = AllocMem((long)sizeof(*Quest),MEMF_PUBLIC);
   DoCloseMap();
   if(passc > 0) return(1);
   return(Security());
   }


ShowTitlePage(i)
int i;

{
   if(i == 0) LoadSong();
   FadeOut();
   copybuffer(&bit[0],screen);
   restore_colors(&MyScreen->ViewPort,rgbmap0,16L);
   if(i == 2) PlaySong(0);
   }
   
   
   
SetColors(reg,r,g,b)
long reg,r,g,b;

{
   SetRGB4(&MyScreen->ViewPort,reg,r,g,b);
   }


PageFlip(num)
int num;

{
   int x;

   MoveScreen(MyScreen,0L,-400L);
   if(num == 0) {
      /*MyView.LOFCprList = MainC1;
      MyView.SHFCprList = MainC2;*/
      do {
         x = (int)VBeamPos();
         } while((x < 195) || (x > 199));
      LoadView(ViewAddress());
      }
   else {
      MyView.LOFCprList = AltC1;
      MyView.SHFCprList = AltC2;
      do {
         x = (int)VBeamPos();
         } while((x < 195) || (x > 199));
      LoadView(&MyView);
      }
   }


BlitElement(elm,dx,dy,buf,mode)
int elm,dx,dy,mode,buf;

{
   int bn;
   
   if(elm < 143) bn = 1;
   else if(elm < 286) {
      bn = 2;
	  elm -= 143;
	  }
   else {
      bn = 3;
	  elm -= 286;
	  }
   if(buf == 0)
    BltBitMap(&bit[bn],(long)elmx[elm],(long)elmy[elm],screen,
    (long)dx * 24,(long)dy * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   else
    BltBitMap(&bit[bn],(long)elmx[elm],(long)elmy[elm],&buf2,
    (long)dx * 24,(long)dy * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   }



elmtotmp(elm,mode)
int elm,mode;

{
   int bn;
   
   if(elm < 143) bn = 1;
   else if(elm < 286) {
      bn = 2;
	  elm -= 143;
	  }
   else {
      bn = 3;
	  elm -= 286;
	  }
   BltBitMap(&bit[bn],(long)elmx[elm],(long)elmy[elm],&buf2,
    288L,16L,24L,18L,
    (long)mode,(long)0xFF,NULL);
   }



tmptomap(x,y,mode,buf)
int x,y,mode,buf;

{
   if(buf == 0) 
    BltBitMap(&buf2,288L,16L,screen,
    (long)x * 24,(long)y * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   else 
    BltBitMap(&buf2,288L,16L,&buf2,
    (long)x * 24,(long)y * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   }



DrawMasked(elm,x,y,onto,buf)
int elm,x,y,onto,buf;

{
   int moff = 1;
   
   if(elm == 115) moff = 33;
   if(elm == 149) moff = 2;
   if(elm == 151) elm = 150;
   if(elm == 376) moff = -268;
   if(maskinfo[onto-18] != 0) {
      elmtotmp(maskinfo[onto-18],COPY_MINTERM);
	  elmtotmp(elm+moff,0xe0);
	  tmptomap(x,y,0x80,buf);
      elmtotmp(elm,COPY_MINTERM);
	  elmtotmp(maskinfo[onto-18],0x20);
	  tmptomap(x,y,0x60,buf);
	  }
   else {
      BlitElement(elm+moff,x,y,buf,0x80);
      BlitElement(elm,x,y,buf,0x60);
	  }
   }

   
DrawCamoed(f,x,y,onto,buf)
int f,x,y,onto,buf;

{
   elmtotmp(107,COPY_MINTERM);
   elmtotmp(f+1,0xe0);
   if(maskinfo[onto-18] != 0) elmtotmp(maskinfo[onto-18],0xe0);
   tmptomap(x,y,0x80,buf);
   elmtotmp(f,COPY_MINTERM);
   if(maskinfo[onto-18] != 0) elmtotmp(maskinfo[onto-18],0x20);
   elmtotmp(107,0x20);
   tmptomap(x,y,0x60,buf);
   }


   
DrawPlayer(num,x,y,onto,buf)
int num,x,y,onto,buf;

{
   int moff = 1,f;
   
   f = Quest->marine[num].facing;
   f = 152 + f*2;
   if((num == CurMarine) && BeltFlag) f += 16;
   if(Invis[num]) {
      elmtotmp(107,COPY_MINTERM);
	  elmtotmp(f+1,0xe0);
      if(maskinfo[onto-18] != 0) elmtotmp(maskinfo[onto-18],0xe0);
	  tmptomap(x,y,0x80,buf);
	  elmtotmp(f,COPY_MINTERM);
      if(maskinfo[onto-18] != 0) elmtotmp(maskinfo[onto-18],0x20);
	  elmtotmp(107,0x20);
	  tmptomap(x,y,0x60,buf);
      }
   else DrawMasked(f,x,y,onto,buf);
   }


   
BlitAnimate(elm,dx,dy,buf,mode)
int elm,dx,dy,mode,buf;

{
   int bn,cx = 0,cy = 0;
   
   if(elm < 143) bn = 1;
   else if(elm < 286) {
      bn = 2;
	  elm -= 143;
	  }
   else {
      bn = 3;
	  elm -= 286;
	  }
   if(dx < 0) cx = -dx;
   if(dy < 0) cy = -dy;
   dx += cx;
   dy += cy;
   if(buf == 0) {
      BltBitMap(&bit[bn],(long)(elmx[elm]+cx),(long)(elmy[elm]+cy),screen,
       (long)dx,(long)dy,(long)(24-cx),(long)(18-cy),
       (long)mode,(long)0xFF,NULL);
	  return;
	  }
   BltBitMap(&bit[bn],(long)(elmx[elm]+cx),(long)(elmy[elm]+cy),&buf2,
    (long)dx,(long)dy,(long)(24-cx),(long)(18-cy),
    (long)mode,(long)0xFF,NULL);
   }



BlitCreature(elm,dx,dy,buf,mode)
int elm,dx,dy,mode,buf;

{
   int bn;
   
   if(elm < 143) bn = 1;
   else if(elm < 286) {
      bn = 2;
	  elm -= 143;
	  }
   else {
      bn = 3;
	  elm -= 286;
	  }
   if(buf == 0)
    BltBitMap(&bit[bn],(long)elmx[elm],(long)elmy[elm],screen,
    (long)dx * 24,(long)dy * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   else
    BltBitMap(&bit[bn],(long)elmx[elm],(long)elmy[elm],&buf2,
    (long)dx * 24,(long)dy * 18 + 16,24L,18L,
    (long)mode,(long)0xFF,NULL);
   }



BlitArrows(x,y)
int x,y;

{
   BlitElement(116,x,y,0,0x80);
   BlitElement(7,x,y,0,0x60);
   }
   
   
   
TracePath(mflag)
Boolean mflag;

{
   Byte path[20][2];
   int PathLen = 1,pl;
   int x,y,dx,dy,ox,oy,z,tx,ty,x1,y1,curelm;
   long c = 14,col = 13;
   int cost = 0;
   Boolean diag,b,trace,b1;

   SetSee(3);
   if(mflag) MyWindow->Flags |= REPORTMOUSE;
   path[0][0] = 4;
   path[0][1] = 4;
   if(GridFlag) DrawAllGrid();
   BlitArrows(4,4);
   do {
      trace = FALSE;
	  c = mygetch();
      if(c == -4) {
         x = MousePos.x / 24;
         y = (MousePos.y - 16) / 18;
         dx = MousePos.x % 24;
         dy = (MousePos.y - 16) % 18;
		 trace = TRUE;
		 }
	  if((c >= 2000) && (c <= 2007)) {
	     x = path[PathLen-1][0];
		 y = path[PathLen-1][1];
		 dx = 12;
		 dy = 9;
		 if(c == 2000) y--;
		 if(c == 2001) x++;
		 if(c == 2002) y++;
		 if(c == 2003) x--;
		 if(c == 2004) { y--; x++; }
		 if(c == 2005) { y++; x++; }
		 if(c == 2006) { y++; x--; }
		 if(c == 2007) { y--; x--; }
		 trace = TRUE;
		 }
      if(trace && (x >= 0) && (y >= 0) && (x < 9) && (y < 9)) {
	     curelm = Quest->map[Level][path[PathLen-1][0]+MapOrigin.x]
		  [path[PathLen-1][1]+MapOrigin.y];
         tx = x - path[PathLen-1][0];
         if(tx < 0) ox = -tx;
         else ox = tx;
         ty = y - path[PathLen-1][1];
         if(ty < 0) oy = -ty;
         else oy = ty;
         if(ox == oy) diag = TRUE;
         else diag = FALSE;
         /* Look for interpolated square */
         x1 = -1;
         if((ox == 2) || (oy == 2)) {
            if(ox == 2) x1 = x - (tx/2);
            else x1 = x;
            if(oy == 2) y1 = y - (ty/2);
            else y1 = y;
            }
         /* Do trace */
         if((ox+oy != 0) && (ox < 3) && (oy < 3)
          && (PathLen < 20) && (dx >= 5) && (dx <= 19)
          && (dy >= 4) && (dy <= 13)) {
            if((PathLen > 1) && (path[PathLen-2][0] == x) &&
             (path[PathLen-2][1] == y)) {
               DrawSquare(path[PathLen-1][0],path[PathLen-1][1],
                0);
               if(GridFlag) DrawGrid(path[PathLen-1][0],path[PathLen-1][1]);
               DrawSquare(path[PathLen-2][0],path[PathLen-2][1],
                0);
               if(GridFlag) DrawGrid(path[PathLen-2][0],path[PathLen-2][1]);
               PathLen--;
               col = 13;
               for(z = 1; z < PathLen; z++) 
                DrawPath(path[z-1][0],path[z-1][1],path[z][0],path[z][1],&col);
			   if(BeltFlag) {
			      cost--;
				  if(diag) cost--;
				  }
			   else {
                  cost -= movc[Quest->map[Level][path[PathLen][0]+MapOrigin.x][path[PathLen][1]+MapOrigin.y]];
                  if(diag) cost -= 
                   movc[Quest->map[Level][path[PathLen][0]+MapOrigin.x]
                   [path[PathLen][1]+MapOrigin.y]] / 2;
				  }
               UpdateMoves(cost);
			   BlitArrows(path[PathLen-1][0],path[PathLen-1][1]);
               }
            else if((ox == oy) || (ox == 0) || (oy == 0)) {
			   pl = PathLen;
               b = TRUE;
			   b1 = FALSE;
               if(x1 != -1) b =
			   PathMove(curelm,tx,ty,path,&PathLen,&cost,&diag,x1,y1,&col);
	           curelm = Quest->map[Level][path[PathLen-1][0]+MapOrigin.x]
	      	    [path[PathLen-1][1]+MapOrigin.y];
               if(b && (PathLen < 20)) {
			      b1 = PathMove(curelm,tx,ty,path,&PathLen,&cost,&diag,x,y,&col);
			      if((x1 == -1) && (!b1)) b = FALSE;
				  }
			   if(b || b1) {
                  DrawSquare(path[pl-1][0],path[pl-1][1],0);
                  if(GridFlag) DrawGrid(path[pl-1][0],path[pl-1][1]);
                  for(z = 1; z < PathLen; z++) 
                   DrawPath(path[z-1][0],path[z-1][1],path[z][0],path[z][1],&col);
			      BlitArrows(path[PathLen-1][0],path[PathLen-1][1]);
				  }
               }
            }
         }
      } while((c != -3) && (c != 2008) && (c != 'P'));
   if(mflag) MyWindow->Flags ^= REPORTMOUSE;
   ox = MapOrigin.x - 4;
   oy = MapOrigin.y - 4;
   SetMarL(Quest->marine[CurMarine].level,Quest->marine[CurMarine].x,
    Quest->marine[CurMarine].y,15);
   SetColor(0);
   MoveTo(0,178);
   DrawTo(216,178);
   DrawTo(216,16);
   for(z = 1; z < PathLen; z++) {
      MapOrigin.x = ox + path[z][0];
      MapOrigin.y = oy + path[z][1];
      dx = path[z][0] - path[z-1][0];
      dy = path[z][1] - path[z-1][1];
      /* Establish new facing */
      if(dx > 0) {
         if(dy < 0) c = 1;
         else if(dy > 0) c = 3;
         else c = 2;
         }
      else if(dx < 0) {
         if(dy < 0) c = 7;
         else if(dy > 0) c = 5;
         else c = 6;
         }
      else {
         if(dy < 0) c = 0;
         else c = 4;
         }
      Quest->marine[CurMarine].facing = c;
      Quest->marine[CurMarine].x = MapOrigin.x+4;
      Quest->marine[CurMarine].y = MapOrigin.y + 4;
      SetMarL(Level,MapOrigin.x+4,MapOrigin.y+4,CurMarine);
      if(CheckPathTText(MapOrigin.x + 4,MapOrigin.y + 4,Level)) break;
      if(CheckPathStun()) break;
      SetMarL(Level,MapOrigin.x+4,MapOrigin.y+4,15);
      }
   CurMove -= cost;
   if(Quest->marine[CurMarine].condition > 0)
    MarineCoord(CurMarine,MapOrigin.x+4,MapOrigin.y+4,Level);
   else CurMove = 0;
   Quest->marine[CurMarine].vitality -= cost;
   DrawMapWindow();
   DrawMarineWindow();
   }



int GetDiskStat(diskreq)
struct IOExtTD *diskreq;

{
   diskreq->iotd_Req.io_Command = TD_CHANGESTATE;
   DoIO(diskreq);
   return((int)diskreq->iotd_Req.io_Actual);
   }



Boolean WantDisk(i,skip)
int i;
Boolean skip;

{
   struct FileLock *my_lock, *Lock() ;
   struct IOExtTD *diskreq;
   struct MsgPort *diskport;
   char str[15];
   int x;
   Boolean succ = TRUE;

   if(i == 0) strcpy(str,"DF0:Breach2");
   else strcpy(str,"");
   diskport = CreatePort("diskreq.port",0L);
   diskreq = (struct IOExtTD *)CreateExtIO(diskport,(long)sizeof(struct IOExtTD));
   OpenDevice(TD_NAME,0L,diskreq,0L);
   x = 1;
   while(x != 0) {
      SelectMShow(14+i);
      while(GetDiskStat(diskreq) == 0);
      while(GetDiskStat(diskreq) != 0);
      SelectMClear();
      Delay(110L);
      my_lock = Lock("DF0:",(long)ACCESS_READ);
	  if(strcmp(str,"") == 0) x = 0;
	  else x = access(str,0);
	  UnLock(my_lock);
	  if((x != 0) && skip) {
	     succ = FALSE;
		 break;
		 }   
      }
   CloseDevice(diskreq);
   DeleteExtIO(diskreq,(long)sizeof(struct IOExtTD));
   DeletePort(diskport);
   return(succ);
   }



Boolean CopyScenarios()

{
   FILE *myfile;
   struct FileInfoBlock *FileInfo;
   struct FileLock *mylock, *Lock();
   char fname[80];
   int flen[38];
   int count = 0,code,y,z,yt,infin;
   char *x;
   char files[38][13];
   struct QstStru *fdata[38];
   int datanum;
   int numin,numout;
   Boolean swap = FALSE;

   if(!WantDisk(1,TRUE)) {
      SelectMessage(18);
	  if(swap) WantDisk(0,FALSE);
	  return(FALSE);
	  }
   ShapeCursor(1);
   SelectMShow(17);
   FileInfo = AllocMem((long)sizeof(*FileInfo),MEMF_PUBLIC);
   mylock = Lock("DF0:",(long)ACCESS_READ);
   code = Examine(mylock,FileInfo);
   if(code != 0) do {
      x = rindex(FileInfo->fib_FileName,'.');
      if((x != NULL) && (strcmp(x,".b2s") == 0)) {
         strcpy(files[count++],FileInfo->fib_FileName);
         }
      code = ExNext(mylock,FileInfo);
      } while((code != 0) && (count < 38));
   UnLock(mylock);
   FreeMem(FileInfo,(long)sizeof(*FileInfo));
   fdata[0] = Quest;
   for(datanum = 1; datanum < 38; datanum++) {
      fdata[datanum] = AllocMem((long)sizeof(*Quest),0L);
	  if(fdata[datanum] == NULL) break;
	  }
   y = 0;
   do {
      if((y != 0) && swap) {
	     SelectMClear();
		 ShapeCursor(0);
	     WantDisk(1,FALSE);
		 ShapeCursor(1);
         SelectMShow(17);
		 }
      yt = 0;
      if(count > 0) for(z = 0; z < datanum; z++) {	  
         strcpy(fname,"DF0:");
         strcat(fname,files[y]);
         myfile = fopen(fname,"r");
         numin = fread(fdata[z],1,(int)sizeof(*Quest),myfile);
		 flen[z] = numin;
         fclose(myfile);
		 yt++;
		 y++;
		 if(y == count) break;
		 }
	  if(swap) {
	     SelectMClear();
		 ShapeCursor(0);
	     WantDisk(0,FALSE);
		 ShapeCursor(1);
         SelectMShow(17);
		 }
	  if(count > 0) for(z = 0; z < yt; z++) {
         myfile = fopen(files[y-yt+z],"w");
         numout = fwrite(fdata[z],1,(int)flen[z],myfile);
         fclose(myfile);
         if(flen[z] != numout) {
            ScreenToFront(MyScreen);
            DeleteFile(files[y-yt+z]);
			SelectMClear();
            ShapeCursor(0);
            SelectMessage(16);
            ShapeCursor(1);
            SelectMShow(17);
            y = count;
			break;
            }
         else {
            strcpy(fname,files[y-yt+z]);
            strcat(fname,".info");
            myfile = fopen(fname,"w");
            numout = fwrite(SceInfo,1,SceLen,myfile);
            fclose(myfile);
            if(numout != SceLen) {
               ScreenToFront(MyScreen);
			   DeleteFile(fname);
               DeleteFile(files[y-yt+z]);
	   		   SelectMClear();
               ShapeCursor(0);
               SelectMessage(16);
               ShapeCursor(1);
               SelectMShow(17);
               y = count;
			   break;
               }
            }
         }
      } while((y < count) && (count > 0));
   for(y = 1; y < datanum; y++) FreeMem(fdata[y],(long)sizeof(*Quest));
   return(TRUE);
   }



SetupSelect()

{
   if(IntFlag) {
      RemIntServer(INTB_VERTB,MyInt); 
	  IntFlag = FALSE;
	  }
   My2Screen = MyScreen;
   TempWindow = MyWindow;
   MyScreen = OpenScreen(&Plop2NewScreen);
   FadeOut();
   ShowTitle(MyScreen,0L);
   screen = &MyScreen->BitMap;
   My2NewWindow.Screen = MyScreen;
   MyWindow = OpenWindow(&My2NewWindow);
   MyRast = MyWindow->RPort;
   ScreenToFront(MyScreen);
   BltBitMap(&bit[4],0L,12L,screen,0L,12L,640L,188L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }



SetupGame()

{
   if(passc == 0) {
      CloseWindow(MyWindow);
      CloseScreen(MyScreen);
      MyScreen = My2Screen;
      MyWindow = TempWindow;
      AddIntServer(INTB_VERTB,MyInt);
  	  IntFlag = TRUE;
	  }
   MyRast = MyWindow->RPort;
   My2Screen = NULL;
   screen = &MyScreen->BitMap;
   }
   
   
Fade2In()

{
   restore_colors(&MyScreen->ViewPort,rgbmap2,16L);
   }
   
   
fdelete(fname)
char *fname;

{
   char astr[40];
   
   strcpy(astr,fname);
   DeleteFile(astr);
   strcat(astr,".info");
   DeleteFile(astr);
   }
   
   
   
ReadScenarios()

{
   FILE *myfile;
   struct FileInfoBlock *FileInfo;
   struct FileLock *mylock, *Lock();
   int code,y;
   char *x;
   Boolean swap;

   counts[0] = 0;
   sstart = 0;
   FileInfo = AllocMem((long)sizeof(*FileInfo),MEMF_PUBLIC);
   mylock = Lock(fpath,(long)ACCESS_READ);
   code = Examine(mylock,FileInfo);
   if(code != 0) do {
      x = rindex(FileInfo->fib_FileName,'.');
      if((x != NULL) && (strcmp(x,".b2s") == 0)) {
         myfile = fopen(FileInfo->fib_FileName,"r");
         fread(&scennames[counts[0]][1],1,20,myfile);
		 scennames[counts[0]][21] = 0;
         fread(&scendifs[counts[0]],1,1,myfile);
         scennames[counts[0]][0] = scendifs[counts[0]] + 1;
         strcpy(scenfile[counts[0]],FileInfo->fib_FileName);
         fclose(myfile);
         counts[0]++;
         }
      code = ExNext(mylock,FileInfo);
      } while((code != 0) && (counts[0] < 38));
   UnLock(mylock);
   FreeMem(FileInfo,(long)sizeof(*FileInfo));
   if(counts[0] < 2) return;
   y = 0;
   do {
	  swap = FALSE;
	  while((strcmp(scennames[y],scennames[y+1]) > 0) && ((y+1) < counts[0])) {
	     swapmem(scennames[y],scennames[y+1],22);
		 swapmem(&scendifs[y],&scendifs[y+1],1);
		 swapmem(scenfile[y],scenfile[y+1],13);
		 swap = TRUE;
		 y++;
		 }
	  if(swap) y = 0;
	  else y++;
      } while((y+1) < counts[0]);	  
   }



ReadGames()

{
   FILE *myfile;
   struct FileInfoBlock *FileInfo;
   struct FileLock *mylock, *Lock();
   int code;
   char *x;

   counts[2] = 0;
   FileInfo = AllocMem((long)sizeof(*FileInfo),MEMF_PUBLIC);
   mylock = Lock(fpath,(long)ACCESS_READ);
   code = Examine(mylock,FileInfo);
   if(code != 0) do {
      x = rindex(FileInfo->fib_FileName,'.');
      if((x != NULL) && (strcmp(x,".b2g") == 0)) {
         myfile = fopen(FileInfo->fib_FileName,"r");
         fread(gamenames[counts[2]],1,20,myfile);
         fread(gamescen[counts[2]],1,20,myfile);
		 gamescen[counts[2]][20] = 0;
         fread(gamelead[counts[2]],1,13,myfile);
         strcpy(gamefile[counts[2]],FileInfo->fib_FileName);
         fclose(myfile);
         counts[2]++;
         }
      code = ExNext(mylock,FileInfo);
      } while((code != 0) && (counts[2] < 4));
   UnLock(mylock);
   FreeMem(FileInfo,(long)sizeof(*FileInfo));
   }



int ReadLeaders()

{
   FILE *myfile;
   struct FileInfoBlock *FileInfo;
   struct FileLock *mylock, *Lock();
   char temp1[20],temp2[20];
   int code,y;
   char *x;

   counts[1] = 0;
   FileInfo = AllocMem((long)sizeof(*FileInfo),MEMF_PUBLIC);
   mylock = Lock(fpath,(long)ACCESS_READ);
   code = Examine(mylock,FileInfo);
   if(code != 0) do {
      x = rindex(FileInfo->fib_FileName,'.');
      if((x != NULL) && (strcmp(x,".b2l") == 0)) {
         myfile = fopen(FileInfo->fib_FileName,"r");
         fread(temp1,1,20,myfile);
         strptoc(temp2,temp1);
         strcpy(&(leadnames[counts[1]][0]),&(temp2[3]));
         strcpy(leadgame[counts[1]],"Available");
         strcpy(leadfile[counts[1]],FileInfo->fib_FileName);
         for(y = 0; y < counts[2]; y++)
          if(strcmp(leadfile[counts[1]],gamelead[y]) == 0)
          strcpy(leadgame[counts[1]],gamenames[y]);
         fclose(myfile);
         counts[1]++;
         }
      code = ExNext(mylock,FileInfo);
      } while((code != 0) && (counts[1] < 4));
   UnLock(mylock);
   FreeMem(FileInfo,(long)sizeof(*FileInfo));
   if((counts[1] == 4) && (code != 0)) return(1);
   return(0);
   }



CreateLeader(lname,mname)
char *lname;
Boolean mname;

{
   int choice,num;
   char fname[80],s[80];
   FILE *theFile;
   struct DiskObject *dobj;

   if(mname) {
      ShapeCursor(1);
      strcpy(fname,"EN ");
      strncat(fname,lname,16);
      fname[19] = '\000';
      strctop(Paladin.name,fname);
      SetSLDefault();
	  }
   else {
      strptoc(fname,Paladin.name);
	  strcpy(lname,&fname[3]);
	  }
   strncpy(fname,lname,8);
   fname[8] = '\000';
   strcat(fname,".b2l");
   SetFileName(fname);
   theFile = fopen(fname,"w");
   strcpy(tfname,fname);
   num = fwrite((unsigned char *)&Paladin,1,sizeof(Paladin),theFile);
   if(num != sizeof(Paladin)) SelectMessage(11);
   fclose(theFile);
   if(num != sizeof(Paladin)) {
      DeleteFile(fname);
	  strcpy(tfname,"");
	  }
   else {
      strcpy(s,fname);
      strcat(fname,".info");
      theFile = fopen(fname,"w");
      fwrite(SqlInfo,SqlLen,1,theFile);
      fclose(theFile);
      dobj = GetDiskObject(s);
      dobj->do_CurrentX = NO_ICON_POSITION;
      dobj->do_CurrentY = NO_ICON_POSITION;
      PutDiskObject(s,dobj);
      FreeDiskObject(dobj);
      if(mname) {
	     ReadLeaders();
         ListLeaders();
		 }
      }
   if(mname) ShapeCursor(0);
   }



Boolean DoSaveName(gname,sname,lname,fname)
char *fname,*gname,*sname,*lname;

/* Save a Scenario to the disk */

{
   FILE *theFile;
   int num;
   struct DiskObject *dobj;
   char tname[80];

   strcpy(tname,fname);
   theFile = fopen(tname,"w");
   fwrite(gname,1,20,theFile);
   fwrite(sname,1,20,theFile);
   fwrite(lname,1,13,theFile);
   crushit();
   num = fwrite(crushed,1,(int)crulen,theFile);
   fclose(theFile);
   if(num != crulen) {
      DeleteFile(fname);
      return(FALSE);
      }
   strcat(tname,".info");
   theFile = fopen(tname,"w");
   num = fwrite(GamInfo,1,GamLen,theFile);
   fclose(theFile);
   if(num != GamLen) {
      DeleteFile(tname);
	  DeleteFile(fname);
	  return(FALSE);
      }
   dobj = GetDiskObject(fname);
   dobj->do_CurrentX = NO_ICON_POSITION;
   dobj->do_CurrentY = NO_ICON_POSITION;
   PutDiskObject(fname,dobj);
   FreeDiskObject(dobj);
   NewFlag = FALSE;
   Changed = FALSE;
   return(TRUE);
   }



SuspendProgram()

{
   ScreenToBack(MyScreen);
   if(My2Screen != NULL) ScreenToBack(My2Screen);
   }


   
OrderBar(x,y)
int x,y;

{
   BltBitMap(&bit[3],144L,180L,screen,(long)x,(long)y,20L,8L,
    (long)COPY_MINTERM,(long)0xFF,NULL); 
   BltBitMap(&bit[3],220L,180L,screen,(long)(x+20),(long)y,100L,8L,
    (long)COPY_MINTERM,(long)0xFF,NULL); 
   BltBitMap(&bit[3],220L,180L,screen,(long)(x+100),(long)y,100L,8L,
    (long)COPY_MINTERM,(long)0xFF,NULL); 
   }   
   
 
 
ShowWinScreen()

{
   SaveBuffer();
   BltBitMap(&bit[3],0L,126L,screen,90L,60L,140L,62L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   ShapeCursor(0);
   PlaySong(1);
   if(!SoundFlag) KeyWait();
   LoadBuffer();
   }   
   
   
   
BlitInsignia(num,x,y)
int num,x,y;

{
   int sx,sy;
   
   sx = (num % 5) * 24 + 192;
   sy = (num / 5) * 8 + 162;
   BltBitMap(&bit[3],(long)sx,(long)sy,screen,(long)x,(long)y,24L,8L,
    (long)COPY_MINTERM,(long)0xFF,NULL);
   }
   
   
   
SwapDisk()

{
   struct FileLock *my_lock, *Lock(), *CurrentDir();
   struct Info_Data *idata;
   struct IOExtTD *diskreq;
   struct MsgPort *diskport;
   int x,mover = -1,z;
   char dummy[40];
   FILE *myfile;
   
   if(selector[1] != -1) {
      if(SelectYesNo(21) == 1) {
         if(strcmp(leadgame[selector[1]],"Available") != 0) SelectMessage(26);
		 else mover = selector[1];
		 }
      }
   if(mover != -1) {
      myfile = fopen(leadfile[mover],"r");
      fread(&Paladin,1,sizeof(Paladin),myfile);
      fclose(myfile);
	  DeleteLeader(leadfile[mover]);
	  Delay(200L);
      }
   diskport = CreatePort("diskreq.port",0L);
   diskreq = (struct IOExtTD *)CreateExtIO(diskport,(long)sizeof(struct IOExtTD));
   OpenDevice(TD_NAME,0L,diskreq,0L);
   x = 1;
   while(x != 0) {
      SelectMShow(20);
      while(GetDiskStat(diskreq) == 0);
      while(GetDiskStat(diskreq) != 0);
      SelectMClear();
	  ShapeCursor(1);
	  Delay(110L);
      my_lock = Lock("DF0:",(long)ACCESS_READ);
      if(ridlock == NULL) orglock = CurrentDir(my_lock);
	  else CurrentDir(my_lock);
	  if(ridlock != NULL) UnLock(ridlock);
	  ridlock = my_lock;
	  strcpy(fpath,GetCD());
	  ShapeCursor(0);
	  if(mover == -1) x = 0;
	  else {
	     CreateLeader(dummy,FALSE);
	     if(tfname == NULL) SelectMessage(23);
	     else {
		    ShapeCursor(1);
		    counts[2] = 0;
	        z = ReadLeaders();
			ShapeCursor(0);
		    if(z != 0) SelectMessage(24);
		    else x = 0;
			}
		 }
      }
   ShapeCursor(1);
   CloseDevice(diskreq);
   DeleteExtIO(diskreq,(long)sizeof(struct IOExtTD));
   DeletePort(diskport);
   SelectMShow(17);
   ReadScenarios();
   ReadGames();
   ReadLeaders();
   SelectMClear();
   ListScenarios();
   ListLeaders();
   ListGames();
   ShapeCursor(0);
   }



DeleteLeader(s)
char *s;

{
   char fname[40];
   
   DeleteFile(s);
   strcpy(fname,s);
   strcat(fname,".info");
   DeleteFile(fname);
   }
   
   
GifClear()

{
   codesize = initcodesize;
   maxcode = maxcodes[codesize-2];
   freecode = firstfree;
   readmask = masks[codesize-3];
   }



GifPixel(c,i)
char *c;
int i;

{
   int x;
   
   for(x = 0; x < i; x++) {
      if(*c & 0x01) *gifbuf[0] |= bufmask;
      if(*c & 0x02) *gifbuf[1] |= bufmask;
      if(*c & 0x04) *gifbuf[2] |= bufmask;
      if(*c & 0x08) *gifbuf[3] |= bufmask;
	  c--;
	  bufmask >>= 1;
	  if(bufmask == 0) {
	     gifbuf[0]++;
		 gifbuf[1]++;
		 gifbuf[2]++;
		 gifbuf[3]++;
		 bufmask = 128;
		 }
      }
   }



GifCode()

{
   unsigned long a;
   unsigned int byteoffset;

   byteoffset = bitoffset >> 3;
   while(byteoffset > rcount) {
      rbuf[0] = rbuf[1];
      rbuf[1] = rbuf[2];
      if(blocksize == 0) blocksize = getc(giffile);
      rbuf[2] = getc(giffile);
      blocksize--;
      rcount++;
      }
   a = rbuf[0] + ((unsigned int)rbuf[1] << 8);
   if(codesize > 8) a += (unsigned long)rbuf[2] << 16;
   a = a >> (bitoffset & 0x07);
   code = a & readmask;
   bitoffset += codesize;
   }



iswap(i)
unsigned int *i;

{
   *i = (*i >> 8) | (*i << 8);
   }
   
   
int DecodeGif(infile,inbuf)
FILE *infile;
int  inbuf;

{
   int i,j,p;
   char buf[255],t[5];
   unsigned char cmap[256][3];
   int x,y,numcol;
   unsigned int bufsize;
   unsigned char z,m,cr,pixel,backg;
   char c;
   char *tp;
   int blen;

   if(inbuf < 4) blen = 8000;
   else blen = 16000;
   holds[0] = 0;
   holds[1] = 0;
   holds[2] = 0;
   holds[3] = 0;
   giffile = infile;
   gifbuf[0] = bit[inbuf].Planes[0];
   gifbuf[1] = bit[inbuf].Planes[1];
   gifbuf[2] = bit[inbuf].Planes[2];
   gifbuf[3] = bit[inbuf].Planes[3];
   for(x = 0; x < 4; x++) setmem(gifbuf[x],blen,0);
   bufmask = 128;

   /* Set up hasing tables */
   prefix = (unsigned int *)malloc(8192);
   suffix = (unsigned char *)malloc(4096);
   outcode = (unsigned char *)malloc(2000);

   /* Do GIF Signature */
   fread(buf,6,1,giffile);

   /* Do Screen Descriptor */
   fread(&x,2,1,giffile);
   fread(&y,2,1,giffile);
   fread(&z,1,1,giffile);
   iswap(&x); iswap(&y);
   m = z >> 7;
   cr = ((z & 0x70) >> 4) + 1;
   pixel = (z & 0x07) + 1;
   fread(&backg,1,1,giffile);
   fread(buf,1,1,giffile);

   /* Do Global Color Map */
   numcol = 2;
   for(z = 1; z < pixel; z++) numcol *= 2;
   fread(cmap,numcol*3,1,giffile);
   z = 0xf0;
   if(inbuf == 1) {
      for (i=0; i<16; i++) {
         rgbmap1[i] = ((cmap[i][0] & z) << 4);
	     rgbmap1[i] += (cmap[i][1] & z);
         rgbmap1[i] += ((cmap[i][2] & z) >> 4);
		 }
      }
   else if(inbuf < 1) {
      for (i=0; i<16; i++) {
         rgbmap0[i] = ((cmap[i][0] & z) << 4);
	     rgbmap0[i] += (cmap[i][1] & z);
         rgbmap0[i] += ((cmap[i][2] & z) >> 4);
		 }
      }
   else {
      for (i=0; i<16; i++) {
         rgbmap2[i] = ((cmap[i][0] & z) << 4);
	     rgbmap2[i] += (cmap[i][1] & z);
         rgbmap2[i] += ((cmap[i][2] & z) >> 4);
		 }
      }
   /* Ignore Image Descriptor */
   fseek(giffile,10L,1);

   /* Do Raster Data */
   codesize = 0;
   fread(&codesize,1,1,giffile);
   iswap(&codesize);
   fread(&blocksize,1,1,giffile);
   fread(rbuf,1,3,giffile);
   blocksize -= 3;
   rcount = 0;
   gifoff = 0;
   gifline = 0;

   clearcode = pow2[codesize];
   eofcode = clearcode+1;
   firstfree = clearcode + 2;
   freecode = firstfree;
   codesize++;
   initcodesize = codesize;
   maxcode = maxcodes[codesize-2];
   readmask = masks[codesize-3];

   bitoffset = 0;
   outcount = 0;
   bitmask = codemask[pixel];
   do {
      GifCode();
      if(code != eofcode) {
	 if(code == clearcode) {
	    GifClear();
	    GifCode();
	    curcode = code;
	    oldcode = code;
	    finchar = code & bitmask;
        GifPixel(&finchar,1);
	    }
	 else {
	    curcode = code;
	    incode = code;
	    if(code >= freecode) {
	       curcode = oldcode;
	       outcode[outcount++] = finchar;
	       }
	    if(curcode > bitmask) do {
	       outcode[outcount++] = suffix[curcode];
	       curcode = prefix[curcode];
	       } while(curcode > bitmask);
	    finchar = curcode & bitmask;
            outcode[outcount++] = finchar;
            GifPixel(&outcode[outcount-1],outcount);
	    outcount = 0;
	    prefix[freecode] = oldcode;
		suffix[freecode++] = finchar;
	    oldcode = incode;
	    if(freecode >= maxcode) {
	       if(codesize < 12) {
		  codesize++;
		  maxcode <<= 1;
		  readmask = masks[codesize-3];
		  }
	       }
	    }
	 }

      } while(code != eofcode);
   free(prefix);
   free(suffix);
   free(outcode);
   return(TRUE);
   }
   
   
   

   