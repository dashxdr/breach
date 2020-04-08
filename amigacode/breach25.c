/* breach25.c - Sound Effects */

#include "breach2s.h"

typedef LONG ID;    /* An ID is four printable ASCII chars but
      * stored as a LONG for efficient copy & compare.*/
typedef struct {
    ID    ckID;
    LONG  ckSize;
    } ChunkHeader;
 
typedef struct {
    ID    ckID;
    LONG  ckSize;
    UBYTE ckData[ 1 /*REALLY: ckSize*/ ];
    } Chunk;

typedef struct { UWORD tempo;
				 UBYTE volume;
				 UBYTE ctTrack;
			   } SScoreHeader;
			   
typedef struct { UBYTE reg;
			     UBYTE type;
				 UBYTE data1,data2;
				 char name[];
				 } RefInstrument;
				 
typedef struct { UBYTE sID;
                 UBYTE data;
				 } SEvent;
typedef struct {
     ULONG oneShotHiSamples,  /* # samples in the high octave 1-shot part */
   repeatHiSamples, /* # samples in the high octave repeat part */
   samplesPerHiCycle;    /* # samples/cycle in high octave, else 0   */
     UWORD samplesPerSec;     /* data sampling rate    */
     UBYTE ctOctave,         /* # octaves of waveforms     */
     sCompression;       /* data compression technique used */
     LONG volume;    /* playback volume from 0 to Unity (full
           * volume). Map this value into the output
           * hardware's dynamic range.  */
     } Voice8Header;
 
extern struct GfxBase *GfxBase;
extern struct Window *MyWindow;
				 
#define MakeID(a,b,c,d)  ( (LONG)(a)<<24L | (LONG)(b)<<16L | (c)<<8L | (d) )
#define FORM MakeID('F','O','R','M')
#define SMUS MakeID('S','M','U','S')
#define SHDR MakeID('S','H','D','R')
#define INS1 MakeID('I','N','S','1')
#define TRAK MakeID('T','R','A','K')
#define I8SVX MakeID('8','S','V','X')
#define VHDR MakeID('V','H','D','R')
#define BODY MakeID('B','O','D','Y')

#define INLIMIT 8

LONG keepout();

UBYTE *score[3] = {0,0,0};
int insmap[8];
long scorelen[3];
SEvent	*pSEvent;
RefInstrument *pRefInstrument;
SScoreHeader *pSScoreHeader;
Chunk	*pChunk;
char	*playname,inname[60],iobuffer[8];
LONG	x,rdcount,datacount,tscale;
ULONG	pstat;
long inslocs[6],muslocs[3];
int insnum,musnum;
char insname[6][15];
int vol[4] = {60,60,60,60};
UBYTE tied[4];
FILE *player;
UBYTE *psdata;
ChunkHeader *pChunkHeader;
SEvent *ptrak[8];
LONG trakcount;

FILE *inshandle;
ULONG get8();
void kill8svx();

LONG data8count,clock;
UBYTE *p8data;
Voice8Header *pVoice8Header;
Chunk *p8Chunk;
LONG fsize,instrumentno,scount;
UBYTE *sbase,*sbases[INLIMIT];
LONG *ptabptr,*ptabptrs[INLIMIT],ssize,ssizes[INLIMIT],length[16*INLIMIT];
BYTE *psample[16*INLIMIT];
LONG maxfreq[] = {41860,44540,46986,49780,52740,55860,
                  59200,62720,66448,70400,74586,79022};

struct Message *GetMsg();
struct IORequest *CreateExtIO();
struct Task *FindTask();
struct Task *CreateTask();

void notes();
ULONG compute();
void settime();
ULONG setaudio();
void userkill();

struct IOAudio *AudioIOBptr[20];
struct Message *msg;
struct MsgPort *port[4];
struct MsgPort *tport[4];
ULONG device[4];
struct timerequest *treq[8];
ULONG timer[4];

UBYTE inreg[4];
UBYTE chan1[] = { 1 };
UBYTE chan2[] = { 2 };
UBYTE chan3[] = { 4 };
UBYTE chan4[] = { 8 };
UBYTE *chans[] = {chan1,chan2,chan3,chan4};
ULONG ttable[64];
ULONG otable[] = {26880,13440,6720,3360,1680,840,420,210,
                  40320,20160,10080,5040,2520,1260,630,315,
				  17920,8960,4480,2240,1120,560,280,140,
				  26880,13440,6720,3360,1680,840,420,210,
				  21504,10752,5376,2688,1344,672,336,168,
				  32256,16128,8064,4032,2016,1008,504,252,
				  23040,11520,5760,2880,1440,720,360,180,
				  34560,17280,8640,4320,2160,1080,540,270};

/* Defines */
#define LEFT0F         1
#define RIGHT0F        2
#define RIGHT1F        4
#define LEFT1F         8

/* Global Variables */
int smode = 1;
BYTE *sndbuf[19],*sndmain;
long  sndlen[19],sndmax = 0;
int  sndrat[19];
int sndnum,sndload = -1;
BYTE sndpri[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
struct IOAudio *id = NULL; 
UBYTE allocationMap[] = {LEFT0F,LEFT1F,RIGHT0F,RIGHT1F};

/* External Variables */
extern Boolean SoundFlag;

/* External Procedures */
struct MsgPort *CreatePort();



/* Allocate and initialize an IO Request Block */
SetIOA(per,vol,repeat,len,ioa)
long per,vol,repeat;
ULONG len;
struct IOAudio **ioa;

{
   struct MsgPort *port;
   char *PortName;

   PortName = "Sound Channel";

   /* Allocate IOAudio structure */
   (*ioa) = (struct IOAudio *)AllocMem((long)sizeof(struct IOAudio),
    (long)MEMF_PUBLIC|MEMF_CLEAR);

   /* If Allocation Successful ... */
   if((*ioa)!=0) {
       /* Set Priority */
       (*ioa)->ioa_Request.io_Message.mn_Node.ln_Pri = 10;
       /* Create Message port for IORequest to talk to Amiga */
       if ((port = CreatePort(PortName,0L)) == 0)
        FreeMem((*ioa),(long)sizeof(struct IOAudio));
       /* If Creation Successful ... */
       else {
           /* Get a channel */
           (*ioa)->ioa_Request.io_Message.mn_ReplyPort = port;
           (*ioa)->ioa_Data = allocationMap;
           (*ioa)->ioa_Length = sizeof(allocationMap);
           /* Open Audio Device for output */
           if(OpenDevice(AUDIONAME,0L,(*ioa),0L) != 0)
           {
               DeletePort(port);
               FreeMem((*ioa),(long)sizeof(struct IOAudio));
           }
           /* If open worked ... */
           else
           {
               /* Set Up Request */
               (*ioa)->ioa_Request.io_Flags = ADIOF_PERVOL;
               (*ioa)->ioa_Request.io_Command = CMD_WRITE;
               (*ioa)->ioa_Period = per;
               (*ioa)->ioa_Volume = vol;
               (*ioa)->ioa_Cycles = repeat;
           }
       }
   }
}



struct IOAudio *PlaySound(buffer,buflen,repeat,period,volume)
BYTE *buffer;
ULONG buflen;
long repeat,period,volume;

{
   struct IOAudio *ioa;
   BYTE *DataPtr;
   ULONG PlayLen=0;
   long sound_done=0;

   DataPtr = buffer;

   /* Set Up IOAudio structure */
   SetIOA(period,volume,repeat,buflen,&ioa);

   /* Set up data and length pointers of ioa */
   SetLength(&PlayLen,buflen,&DataPtr,ioa);

   /* Send command to Audio chip */
   BeginIO(ioa);
   return(ioa);
   }



StopSound(ioa)
struct IOAudio *ioa;

{
   AbortIO(ioa);
   if(ioa->ioa_Request.io_Device) CloseDevice(ioa);
   if(ioa->ioa_Request.io_Message.mn_ReplyPort)
    DeletePort(ioa->ioa_Request.io_Message.mn_ReplyPort);
   if(ioa) FreeMem(ioa,(long)sizeof(struct IOAudio));
   id = NULL;
   }



SetLength(LenPtr,buflen,DataHndl,ioa)
ULONG *LenPtr,buflen;
BYTE **DataHndl;
struct IOAudio *ioa;

{
   (*LenPtr) = buflen;
   ioa->ioa_Length = (*LenPtr);
   ioa->ioa_Data = (*DataHndl);
   (*LenPtr) = 0L;
   }  



FixLength(LenPtr,DataHndl,ioa)
ULONG *LenPtr;
BYTE **DataHndl;
struct IOAudio *ioa;

{
   ioa->ioa_Length = (*LenPtr);
   ioa->ioa_Data = (*DataHndl);
   (*LenPtr) = 0L;
   }



SoundStop()

{
   if(id == NULL) return;
   if(smode == 1) do { } while(!CheckIO(id));
   StopSound(id);
   }


SoundEff(sid)
int sid;

{
   long scont;
   int x;

   if(sid == -1) {
      SoundStop();
      smode = 1;
      return;
      }
   if(smode == 0) return;
   if(id != NULL) {
      if(smode == 1) do { } while(!CheckIO(id));
      StopSound(id);
      }
   if(sid > sndnum) return;
   if(sndlen[sid-1] == 0) return;
   if(!SoundFlag) return;
   if(sid == -1) smode = 1;
   else {
      if(sid > 17) smode = 0;
	  else smode = 1;
	  }
   if(smode == 0) scont = 0;
   else scont = 1;
   if(sndpri[sid-1]) {
      id=PlaySound(sndbuf[sid-1],sndlen[sid-
       1],scont,(long)3579545L/(long)sndrat[sid-1],64L);
	  return;
	  }
   if(sndload != sid) {
      sndload = sid;
	  CopyMem(sndbuf[sid-1],sndmain,sndlen[sid-1]);
	  }
   id=PlaySound(sndmain,sndlen[sid-
    1],scont,(long)3579545L/(long)sndrat[sid-1],64L);
   }


LoadSong()

{  
   scount = 0;instrumentno = 0;pstat = 1;
   player = fopen("breach2.mus","r");
   fread(&musnum,1,2,player);
   for(x = 0; x < musnum; x++) fread(&muslocs[x],1,4,player);
   inshandle = fopen("breach2.ins","r");
   fread(&insnum,1,2,inshandle);
   for(x = 0; x < insnum; x++) fread(&insname[x][0],1,15,inshandle);
   for(x = 0; x < insnum; x++) fread(&inslocs[x],1,4,inshandle);
  
   fseek(player,0L,2);
   scorelen[0] = muslocs[1] - muslocs[0];
   scorelen[1] = muslocs[2] - muslocs[1];
   scorelen[2] = (ftell(player)+1) - muslocs[2];
   score[0] = (UBYTE *)AllocMem(scorelen[0],MEMF_PUBLIC);
   score[1] = (UBYTE *)AllocMem(scorelen[1],MEMF_PUBLIC);
   score[2] = (UBYTE *)AllocMem(scorelen[2],MEMF_PUBLIC);
   fseek(player,muslocs[0],0);
   fread(score[0],1,(int)scorelen[0],player);
   fread(score[1],1,(int)scorelen[1],player);
   fread(score[2],1,(int)scorelen[2],player);
   fclose(player);
   get8("ElecBass");
   get8("Percussion");
   get8("HollowSynth");
   get8("JazzGuit");
   get8("Trumpet");
   get8("LoString");
   if(inshandle != 0) fclose(inshandle);
   }
   

PlaySong(i)
int i;

{
   char *scoreptr;
   int x,mapper = 0;
   
   if(!SoundFlag) return;
   if(i == 0) ShapeCursor(0);
   scoreptr = (char *)score[i];
   movmem(scoreptr,iobuffer,8);
   scoreptr += 8;
   
   for(x = 0; x < 64; x++) ttable[x] = otable[x];
   pChunkHeader = (ChunkHeader *)iobuffer;
   psdata = (UBYTE *) AllocMem(pChunkHeader->ckSize,MEMF_PUBLIC|MEMF_CLEAR);
   movmem(scoreptr,psdata,(int)pChunkHeader->ckSize);
   scoreptr += (int)pChunkHeader->ckSize;
   datacount = 4;
   trakcount = 0;
   while(datacount < pChunkHeader->ckSize) {
      pChunk = (Chunk *)(psdata+datacount);
	  switch(pChunk->ckID) {
	     case SHDR:
		    pSScoreHeader = (SScoreHeader *)(psdata+datacount+8L);
			tscale = (8929 / (pSScoreHeader->tempo >> 7L));
			for(x = 0; x < 64; x++) ttable[x] *= (ULONG)tscale;
			break;
		 case INS1:
		    pRefInstrument=(RefInstrument *)(psdata+datacount+8L);
			for(x = 0; x <= pChunk->ckSize; x++) 
			 inname[x] = pRefInstrument->name[x];
			inname[pChunk->ckSize-4L] = 0;
            for(x = 0; x < insnum; x++) {
               if(strcmp(inname,insname[x]) == 0) break;
	           }
			insmap[mapper] = x;
			mapper++;
			pstat = 0;
			break;
		 case TRAK:
		    if(trakcount < 4) {
			   pSEvent = (SEvent *)(psdata+datacount+8L);
			   ptrak[trakcount] = pSEvent;
			   ptrak[trakcount+4] = 
			    (SEvent*)(psdata+datacount+8L+pChunk->ckSize);
			   trakcount++;
			   }
			pSEvent = (SEvent*)(psdata+datacount+8L+pChunk->ckSize);
			break;
		 default:
		    break;
		 }
	  datacount += 8L + pChunk->ckSize;
	  if(pChunk->ckSize&1L == 1) datacount++;
	  }
   if(pstat == 0 && instrumentno != 0) {
      pstat = setaudio();
	  if(pstat == 0) notes(ptrak);
	  killaudio();
	  }
   if(psdata != 0L) FreeMem(psdata,pChunkHeader->ckSize);
   if(i == 0) {
      if(ptabptrs[3] != 0) FreeMem(ptabptrs[3],64L);
      if(sbases[3] != 0) FreeMem(sbases[3],ssizes[3]);
	  ShapeCursor(1);
	  }
   }

FreeSong()

{
   kill8svx(NULL);
   for(x = 0; x < 3; x++) if(score[x] != 0) FreeMem(score[x],scorelen[x]);
   }
   
   
ULONG get8(fname)
char *fname;

{
   char iobuffer[8];
   LONG rd8count,ifreq,freq,x,y,midin,ocyc,both,hic;
   UBYTE *tempptr;
   int f1,f2;
   
   if(instrumentno >= INLIMIT) return(0L);
   if(instrumentno == 0L) {
      for(x = 0; x < INLIMIT; x++) {
	     ptabptrs[x] = 0L;
		 sbases[x] = 0L;
		 }
	  if(GfxBase->DisplayFlags & PAL) clock = 35468950L;
	  else clock = 35795450L;
	  }
   p8data = 0;
   data8count = 0;
   
   for(x = 0; x < insnum; x++) {
      if(strcmp(fname,insname[x]) == 0) break;
	  }
   fseek(inshandle,inslocs[x],0);
   rd8count = fread(iobuffer,1,8,inshandle);
   p8Chunk = (Chunk *)iobuffer;
   fsize = p8Chunk->ckSize;
   p8data = (UBYTE *)AllocMem(fsize,MEMF_PUBLIC|MEMF_CLEAR);
   if(fsize > 30000) {
      f1 = 30000;
	  f2 = fsize - 30000;
	  }
   else {
      f1 = fsize;
	  f2 = 0;
	  }
   rd8count = fread(p8data,1,f1,inshandle);
   if(f2 != 0) rd8count += fread(&p8data[30000],1,f2,inshandle);
   data8count = 4;
   while(data8count < fsize) {
      p8Chunk = (Chunk *)(p8data+data8count);
	  switch(p8Chunk->ckID) {
	     case VHDR:
		    pVoice8Header = (Voice8Header *)(p8data+data8count+8L);
			ocyc = pVoice8Header->samplesPerHiCycle;
			if(ocyc == 0) ocyc = 10;
			ifreq = pVoice8Header->samplesPerSec / ocyc;
			y = 0;
			x = 1;
			ifreq *= 10L;
			while(ifreq > 535L*x) {x *= 2L; y++; }
			freq = 275L*x;
			freq = (freq * 103L)/100L;
			midin = 21L + (12L * y);
			for(x = 0; x < 11; x++) {
			   if(ifreq > freq) {
			      freq = (freq * 106L) / 100L;
				  midin++;
				  }
			   else x = 12;
			   }
			ifreq = ifreq / 10L;
			ptabptr = (LONG *)AllocMem(64L,MEMF_PUBLIC|MEMF_CLEAR);
			ptabptrs[instrumentno] = ptabptr;
			x = 120;
			while(midin < x) x = x - 12;
			hic = x;
			x = (108-hic)/12;
			y = 1;
			while(x > 0) { y *= 2; x--; }
			for(x = 0; x < 12; x++) ptabptr[x] = (clock * y) / maxfreq[x];
			
			for(x = 0; x < 12; x++) ptabptr[x] /= ocyc;
			break;
			
		 case BODY:
		    sbase = (UBYTE *)AllocMem(ssize = p8Chunk->ckSize,MEMF_CHIP|MEMF_CLEAR);
			ssizes[instrumentno] = ssize;
			sbases[instrumentno] = sbase;
			
			tempptr = p8data+data8count+8L;
			for(x = 0; x < ssize; x++) *(sbase+x) = *(tempptr+x);
			y = 1;
			for(x = 0; x < 8; x++) {
			   both = pVoice8Header->oneShotHiSamples +
			    pVoice8Header->repeatHiSamples;
			   both = (both * (y-1)) + (LONG)sbase;
			   psample[scount] = (BYTE *)both;
			   psample[scount+1] = (BYTE *) both +
			    (y * pVoice8Header->oneShotHiSamples);
			   length[scount] = y*pVoice8Header->oneShotHiSamples;
			   length[scount+1] = y*pVoice8Header->repeatHiSamples;
			   if(length[scount] == 0) length[scount] = length[scount+1];
			   if(length[scount+1] == 0) {
			      length[scount+1] = length[scount];
				  psample[scount+1] = psample[scount];
				  }
			   scount++;
			   scount++;
			   if((9-x <= hic/12) && 
			    (9-x > hic/12-pVoice8Header->ctOctave+1)) y *= 2;
			   }
			break;
		 default:
		    break;
	     }
      data8count += 8L + p8Chunk->ckSize;
	  if(p8Chunk->ckSize&1L == 1) data8count++;
	  }
   instrumentno++;
   if(p8data != 0) {
      FreeMem(p8data,fsize);
	  p8data = 0;
	  data8count = 0;
	  }
   return(0L);
   }
   
void kill8svx(kill8svxstring)
char *kill8svxstring;

{
   UBYTE x;
   
   if(inshandle != 0) fclose(inshandle);
   if(p8data != 0) FreeMem(p8data,fsize);
   for(x = 0; x < INLIMIT; x++) if(x != 3) {
      if(ptabptrs[x] != 0) FreeMem(ptabptrs[x],64L);
	  if(sbases[x] != 0) FreeMem(sbases[x],ssizes[x]);
	  }
   }


void notes(trak)
SEvent **trak;

{
   ULONG pstat,wakebit,waitmask;
   ULONG mbits[8];
   UBYTE x,chn,donecount,odev[4];
/*   struct Task *mt;*/
   
   do {
      } while(GetMsg(MyWindow->UserPort));
   if(trakcount > 4) trakcount = 4;
   for(chn = 0; chn < 4; chn++) inreg[chn] = insmap[chn];
   if(instrumentno < 4) {
      for(chn = instrumentno; chn < 4; chn++) inreg[chn] = 0;
	  }
   
   mbits[0] = (1L << tport[0]->mp_SigBit);
   mbits[1] = (1L << tport[1]->mp_SigBit);
   mbits[2] = (1L << tport[2]->mp_SigBit);
   mbits[3] = (1L << tport[3]->mp_SigBit);
   
   mbits[4] = (1L << port[0]->mp_SigBit);
   mbits[5] = (1L << port[1]->mp_SigBit);
   mbits[6] = (1L << port[2]->mp_SigBit);   
   mbits[7] = (1L << port[3]->mp_SigBit);
   
   waitmask = mbits[0] | mbits[1] | mbits[2] | mbits[3] |
    mbits[4] | mbits[5] | mbits[6] | mbits[7];
	
/*   mt = FindTask(NULL);
   SetTaskPri(mt,(unsigned char)22);*/
   for(chn = 0; chn < trakcount; chn++) {
      odev[chn] = 0;
	  pstat = compute(trak,chn,odev[chn]);
	  AudioIOBptr[chn]->ioa_Request.io_Command = CMD_WRITE;
	  AudioIOBptr[chn]->ioa_Request.io_Flags = ADIOF_PERVOL;
	  
	  *AudioIOBptr[chn+4] = *AudioIOBptr[chn];
	  *AudioIOBptr[chn+8] = *AudioIOBptr[chn];
	  *AudioIOBptr[chn+12] = *AudioIOBptr[chn];
	  *AudioIOBptr[chn+16] = *AudioIOBptr[chn];
	  AudioIOBptr[chn+16]->ioa_Request.io_Command = ADCMD_FINISH;
	  AudioIOBptr[chn+16]->ioa_Request.io_Flags = IOF_QUICK;
	  }
   for(chn = 0; chn < trakcount; chn++) {
      odev[chn] = 4;
	  pstat = compute(trak,chn,odev[chn]);
	  }
   for(chn = 0; chn < trakcount; chn++) {
      BeginIO(AudioIOBptr[chn+8]);
	  BeginIO(AudioIOBptr[chn]);
	  }

   for(chn = 0; chn < trakcount; chn++) SendIO(treq[chn]);
   donecount = 0;
   while(donecount < trakcount) {
      wakebit = 0L;
	  while(wakebit == 0L) {
	     wakebit = Wait(waitmask);
		 for(x = 0; x < trakcount; x++) {
		    if(wakebit & mbits[x+4]) {
			   msg = GetMsg(port[x]);
			   wakebit &= ~mbits[x+4];
			   }
			}
	     }
      for(x = 0; x < trakcount; x++) {
	     if(wakebit & mbits[x]) {
		    chn = x + odev[x];
			BeginIO(AudioIOBptr[16+x]);
			BeginIO(AudioIOBptr[16+x]);
			BeginIO(AudioIOBptr[chn+8]);
			BeginIO(AudioIOBptr[chn]);
			SendIO(treq[chn]);
			}
		 }
      while(wakebit != 0) {
	     if(wakebit & mbits[0]) {chn=0;wakebit &= ~mbits[0];}
	     else if(wakebit & mbits[1]) {chn=1;wakebit &= ~mbits[1];}
	     else if(wakebit & mbits[2]) {chn=2;wakebit &= ~mbits[2];}
	     else if(wakebit & mbits[3]) {chn=3;wakebit &= ~mbits[3];}
		 msg = GetMsg(tport[chn]);
		 msg = GetMsg(port[chn]);
		 
		 if(odev[chn] == 4) odev[chn] = 0;
		 else odev[chn] = 4;
		 
		 pstat = compute(trak,chn,odev[chn]);
		 if(pstat == 1) donecount++;
         if(GetMsg(MyWindow->UserPort)) { 
		    userkill();
			return;
			}
		 }
	  }
   while(donecount < (2*trakcount)) {
      wakebit = Wait(waitmask);
	  for(x = 0; x < trakcount; x++) {
	     if(wakebit & mbits[x+4]) msg = GetMsg(port[x]);
		 }
	  for(x = 0; x < trakcount; x++) {
	     if(wakebit & mbits[x]) {
		    BeginIO(AudioIOBptr[16+x]);
		    BeginIO(AudioIOBptr[16+x]);
			donecount++;
			}
		 }
	  }
   for(chn = 0; chn < trakcount; chn++) {
      msg = GetMsg(tport[chn]);
	  msg = GetMsg(port[chn]);
	  }
   }


ULONG compute(track,ch,odev)
SEvent **track;
UBYTE ch,odev;

{
   SEvent *cevent;
   UBYTE psamin,aiob,note,dur;
   struct IOAudio *AAptr;
   struct IOAudio *Aptr;
   struct timerequest *tr;
   ULONG micro,sec;
   
   aiob = ch + odev;
   AAptr = AudioIOBptr[aiob+8];
   Aptr = AudioIOBptr[aiob];
   tr = treq[aiob];
   cevent = track[ch];
   
   while(cevent->sID > 128 || cevent->data > 127) {
      if(cevent->sID == 129 & cevent->data < instrumentno)
	   inreg[ch] = insmap[cevent->data];
	  if(cevent->sID == 132) {
	     vol[ch] = cevent->data;
		 }
	  track[ch]++;
	  cevent++;
	  if(track[ch] >= track[ch+4]) {
	     note = 128;
		 goto DONE;
		 }
	  }
   note = cevent->sID;
   
   DONE:
   
   dur = cevent->data;
   Aptr->ioa_Volume = vol[ch];
   tied[ch] = dur & 64;
   dur &= 63;
   if(note == 128) {
	  Aptr->ioa_Volume = 0;
	  note = 90;
	  }
   AAptr->ioa_Volume = Aptr->ioa_Volume;
   psamin = 9;
   while(note>11) {note -= 12; psamin--; }
   psamin = (psamin << 1L) + (inreg[ch] << 4L);
   
   Aptr->ioa_Data = (UBYTE *)psample[psamin+1];
   Aptr->ioa_Length = (length[psamin+1]);
   AAptr->ioa_Data = (UBYTE *)psample[psamin];
   AAptr->ioa_Length = (length[psamin]);

   ptabptr = ptabptrs[inreg[ch]];
   Aptr->ioa_Period = ptabptr[note];
   AAptr->ioa_Period = ptabptr[note];
   
   sec = 0;
   micro = ttable[dur];
   while(tied[ch]) {
      cevent++;
	  track[ch]++;
      dur = (cevent->data) & 63;
	  tied[ch] = (cevent->data) & 64;
	  micro += ttable[dur];
	  }
   while(micro > 1000000L) {sec++; micro -= 1000000L;}
   tr->tr_node.io_Command=TR_ADDREQUEST;
   tr->tr_time.tv_secs = sec;
   tr->tr_time.tv_micro = micro;
   
   Aptr->ioa_Cycles = 0;
   AAptr->ioa_Cycles = 1;
   
   track[ch]++;
   if(track[ch] > track[ch+4]) return(1L);
   else return(0L);
   }
   
   
killaudio()

{
   UBYTE c;
   
   for(c = 0; c < 4; c++) {
      if(device[c] == 0) CloseDevice(AudioIOBptr[c]);
	  if(timer[c] == 0) CloseDevice(treq[c]);
	  if(port[c] != 0) DeletePort(port[c]);
	  if(tport[c] != 0) DeletePort(tport[c]);
	  if(treq[c] != 0) DeleteExtIO(treq[c]);
	  if(treq[c+4] != 0) DeleteExtIO(treq[c+4]);
	  
	  if(AudioIOBptr[c] != 0)
	   FreeMem(AudioIOBptr[c],(long)sizeof(struct IOAudio));
	  if(AudioIOBptr[c+4] != 0)
	   FreeMem(AudioIOBptr[c+4],(long)sizeof(struct IOAudio));
	  if(AudioIOBptr[c+8] != 0)
	   FreeMem(AudioIOBptr[c+8],(long)sizeof(struct IOAudio));
	  if(AudioIOBptr[c+12] != 0)
	   FreeMem(AudioIOBptr[c+12],(long)sizeof(struct IOAudio));
	  if(AudioIOBptr[c+16] != 0)
	   FreeMem(AudioIOBptr[c+16],(long)sizeof(struct IOAudio));
	  }
   }
   
   
ULONG setaudio()

{
   UBYTE c;
   
   for(c = 0; c < 20; c++) AudioIOBptr[c] = 0;
   for(c = 0; c < 4; c++) {
      treq[c] = 0;
	  treq[c+4] = 0;
	  port[c] = 0;
	  tport[c] = 0;
	  timer[c] = 1;
	  device[c] = 1;
	  }
   for(c = 0; c < 20; c++) {
      AudioIOBptr[c] = (struct IOAudio *)
	   AllocMem((long)sizeof(struct IOAudio),MEMF_CHIP|MEMF_PUBLIC|MEMF_CLEAR);
	  if(AudioIOBptr[c] == 0) return(1L);
	  }
   for(c = 0; c < 4; c++) {
      port[c] = CreatePort(0L,0L);
	  if(port[c] == 0) return(1L);
      AudioIOBptr[c]->ioa_Request.io_Message.mn_ReplyPort = port[c];
      AudioIOBptr[c]->ioa_Request.io_Message.mn_Node.ln_Pri = 21L;
      AudioIOBptr[c]->ioa_AllocKey = 0;
      AudioIOBptr[c]->ioa_Data = chans[c];
      AudioIOBptr[c]->ioa_Length = 1L;
   
      device[c] = OpenDevice(AUDIONAME,0L,AudioIOBptr[c],0L);
      if(device[c] != 0) return(1L);
	  }
   
   for(c = 0; c < 4; c++) {
      tport[c] = CreatePort(0L,0L);
	  if(tport[c] == 0) return(1L);
	  treq[c] = (struct timerequest *)CreateExtIO(tport[c],
	   (long)sizeof(struct timerequest));
	  if(treq[c] == 0) return(1L);
	  treq[c+4] = (struct timerequest *)CreateExtIO(tport[c],
	   (long)sizeof(struct timerequest));
	  if(treq[c+4] == 0) return(1L);
	  
	  timer[c] = OpenDevice(TIMERNAME,UNIT_MICROHZ,treq[c],0L);
	  if(timer[c] != 0) return(1L);
	  
	  *treq[c+4] = *treq[c];
	  }
   return(0L);
   }
	  
 
void userkill()

{
   UBYTE x;
   
   for(x = 16; x < 16+trakcount; x++) BeginIO(AudioIOBptr[x]);
   for(x = 16; x < 16+trakcount; x++) BeginIO(AudioIOBptr[x]);
   for(x = 16; x < 16+trakcount; x++) BeginIO(AudioIOBptr[x]);
   for(x = 16; x < 16+trakcount; x++) BeginIO(AudioIOBptr[x]);
   
   for(x = 0; x < trakcount; x++) AbortIO(treq[x]);
   for(x = 4; x < 4+trakcount; x++) AbortIO(treq[x]);
   for(x = 0; x < trakcount; x++) {
      msg = GetMsg(tport[x]);
	  msg = GetMsg(port[x]);
	  }
   }
   








