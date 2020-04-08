/* #include "stdio.h" */

#define COPY_MINTERM       0xc0
#define srcCopy            0xc0
#define notSrcBic          0x80
#define srcOr              0x60
#define NULL 0L

typedef unsigned char Byte;

typedef unsigned char Boolean;

#define TRUE 255
#define FALSE 0

#define numopp 60
#define numobj 60
	 
/* Take Icon Location */
#define TX1 0
#define TY1 180
#define TX2 47
#define TY2 197

/* Use Icon Location */
#define UX1 48
#define UY1 180
#define UX2 71
#define UY2 197

/* Drop Icon Location */
#define DX1 72
#define DY1 180
#define DX2 119
#define DY2 197

/* Wideview Icon Location */
#define WX1 138
#define WY1 180
#define WX2 161
#define WY2 197

/* Switch Icon Location */
#define NX1 162
#define NY1 180
#define NX2 185
#define NY2 197

/* Open Icon Location */
#define KX1 203
#define KY1 180
#define KX2 250
#define KY2 197

/* Shaft Icon Location */
#define SX1 269
#define SY1 180
#define SX2 316
#define SY2 197

/* Look Icon Location */
#define IX1 224
#define IY1 147
#define IX2 247
#define IY2 164

/* Arm Icon Location */
#define AX1 268
#define AY1 147
#define AX2 315
#define AY2 164

/* List Icon Location */
#define LX1 224
#define LY1 32
#define LX2 315
#define LY2 49

/* Orders Icon Location */
#define OX1 222
#define OY1 12
#define OX2 245
#define OY2 29

/* Briefing Icon Location */
#define BX1 246
#define BY1 12
#define BX2 269
#define BY2 29

/* Round Icon Location */
#define RX1 270
#define RY1 12
#define RX2 293
#define RY2 29

/* Disk Icon Location */
#define FX1 294
#define FY1 12
#define FX2 317
#define FY2 29

struct ptstru {
   int x;
   int y;
   };
typedef struct ptstru Point;

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
