#include "stdio.h"
#include "exec/exec.h"
#include "exec/types.h"
#include   "devices/trackdisk.h"
#include   "exec/io.h"
#include   "ctype.h"
#include   "libraries/dos.h"
#include   "libraries/dosextens.h"
#include   "devices/audio.h"
#include   "exec/exec.h"
#include   "exec/types.h"
#include   "graphics/gfx.h"
#include   "graphics/gfxbase.h"
#include   "graphics/gfxmacros.h"
#include   "graphics/rastport.h"
#include   "graphics/text.h"
#include   "intuition/intuition.h"
#include   "intuition/intuitionbase.h"
#include   "hardware/intbits.h"
#include   "workbench/workbench.h"
#include   "functions.h"
#include   "stdio.h"
#include   "libraries/dos.h"

#define COPY_MINTERM       0xc0
#define srcCopy            0xc0
#define notSrcBic          0xe0
#define srcOr              0x80

typedef unsigned char Byte;
#include "stdio.h"

typedef unsigned char Boolean;

#define TRUE 255
#define FALSE 0

#define numopp 60
#define numobj 60

struct nodestru {
   int x,y;
   struct nodestr *uplink;
   struct nodestr *l1,*l2,*l3,*l4;
   };

struct pathstru {
   int x,y;
   };

/* Paladin structure */
struct PalStru {
   char name[20];
   int class;
   int condition;
   int vitality;
   int maxenc;
   int basemove;
   int bonusmove;
   int shoot;
   int crack;
   int detect;
   int battles;
   int victories;
   long wartime;
   int kills;
   int lost;
   };
#define LeadLen 50L   
   
   
   
/* Quest Structure */
struct MarStru {
   char name[20];
   int class;
   int condition;
   int vitality;
   int encumbrance;
   int maxenc;
   int basemove;
   int bonusmove;
   int shoot;
   int crack;
   int detect;
   int armor;
   int gun;
   Byte inventory[15];
   int x;
   int y;
   int level;
   int facing;
   Boolean psi;
   int ammo;
   };
   
struct OppStru {
   Byte type;
   int  health;
   Byte vitality;
   Byte encumbrance;
   Byte basemove;
   Byte bonusmove;
   Byte armor;
   Byte gun;
   Boolean psi;
   Byte shoot;
   Byte x;
   Byte y;
   Byte level;
   Byte facing;
   };
   
struct ObjStru {
   Byte type;
   Boolean held;
   char itext[30];
   Byte x;
   Byte y;
   Byte level;
   };
   
struct EPoint {
   int x;
   int y;
   int level;
   };

struct TpStru {
   int x;
   int y;
   int level;
   int tox,toy,tolevel;
   };

struct TtStru {
   int x;
   int y;
   int level;
   char itext[40];
   Boolean read;
   };

struct QstStru {
   Byte map[5][50][50];
   Byte vis[50][50];
   struct ObjStru object[numopp];
   struct OppStru opponent[numobj];
   struct MarStru marine[10];
   struct TpStru tport[10];
   struct TtStru ttext[20];
   struct EPoint PEntry;
   struct EPoint PExit;
   char BriefText[540];
   long BriefLen;
   long TimeLimit;
   int KillPercent;
   int OccupyNumber;
   Boolean VCond[7];
   int battles;
   int victories;
   long wartime;
   int kills;
   int lost;
   char slname[253];
   int skill;
   int slshots;
   int slcracks;
   int sldetects;
   int slkills;
   char chain[40];
   };
