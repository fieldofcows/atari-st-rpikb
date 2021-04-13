/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */
#include <stdio.h>

#include "defs.h"
#include "chip.h"
#include "memory.h" /* ireg_getb/putb */
#include "sci.h"
#include "timer.h"
#include "ireg.h"

#ifdef USE_PROTOTYPES
#endif

//#include "options.h"

/*
 * Start/end of internal register block
 */
u_int ireg_start = 0;
u_char  iram[NIREGS];

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/*  ST
    Internal registers are tied to ST hardware. We add functions that
    hopefully should give the correct values when reading DR1, 2 & 4.
    DR3 is never read (ASSERTed).
    Much of this is based on doc translated by Tobe (AF) from French ST Mag 
    articles by Stephane Catala. I also had a look at the original articles.
*/

static u_char dr1_getb P_((u_int offs));
static u_char dr2_getb P_((u_int offs));
static u_char dr4_getb P_((u_int offs));

/*  DR1
    This is the funniest part.
    To read the keyboard, one uses DR1, DR3 & DR4 and their associated direction
    registers DDR1, DDR3 & DDR4.
    All bits of DR1 that are cleared in DDR1 (meaning input) are set. 
    But if a key is pressed in the corresponding row (see table), and the bit in 
    DDR3 or DDR4 corresponding to the column is set, and the corresponding 
    bit in DR3 or DR4 is set (doc said cleared), the bit in DR1 is cleared.
    It's rather complicated, but not hard to emulate. 
    To do that we use a look-up table based on doc. 
    Note that the first row had to be shifted to the right compared with the 
    existing doc.
    We also use a table with key states (already used in Steem), and a function 
    that builds DR1 bit by bit. 
    It's slow but performance isn't needed for the keyboard.
*/

/*  go fetch the value in 6301 rom instead of in a fat table!
    it works, it's fun, but it probably is less efficient than the table,
    and it depends on a precise rom (undef)
f206  1d 2a 38 36: shift etc.
...  
f2f3  80 01 00 02 ce f3 11 d6 8a c1 05 25 0d c0  ...........%..
f301  04 58 58 58 3a 5f 20 01 5c 44 24 fc 3a a6  .XXX:_ .\D$.:.
f30f  00 39 00 00 3b 3c 3d 00 00 00 3e 01 02 0f  .9..;<=...>...
f31d  10 1e 60 2c 3f 03 04 11 12 1f 20 2d 40 05  ..`,?..... -@.
f32b  06 13 14 21 2e 2f 41 07 08 15 22 23 30 31  ...!./A..."#01
f339  42 09 0a 16 17 24 25 32 43 0b 0c 18 19 26  B....$%2C....&
f347  33 39 44 0d 29 1a 1b 27 34 3a 62 0e 53 52  39D.)..'4:b.SR
f355  2b 1c 28 35 61 48 47 4b 50 4d 6d 70 63 64  +.(5aHGKPMmpcd
f363  67 68 6a 6b 6e 71 65 66 69 4a 6c 4e 6f 72  ghjknqefiJlNor
*/

BYTE get_scancode(int dr1bit,int column) {
  BYTE val=0;
//  ASSERT(dr1bit<8);
//  ASSERT(column<15);
//  ASSERT(mem_getb(0xF319)==0x3E);
//  ASSERT(mem_getb(0xF206)==0x1D);
  if(column<4)
  {
    if(!dr1bit)
      val=mem_getb(0xF312+column);
    else if(dr1bit-4==column)
      val=mem_getb(0xF206+column);
  }
  else
    val=mem_getb(0xF319+dr1bit+((column-4)*8));
  return val;
}

/*
SCAN CODES
01  Esc     1B  ]             35  /             4F  { NOT USED }
02  1       1C  RET           36  (RIGHT) SHIFT 50  DOWN ARROW
03  2       1D  CTRL          37  { NOT USED }  51  { NOT USED }
04  3       1E  A             38  ALT           52  INSERT
05  4       1F  S             39  SPACE BAR     53  DEL
06  5       20  D             3A  CAPS LOCK     54  { NOT USED }
07  6       21  F             3B  F1            5F  { NOT USED }
08  7       22  G             3C  F2            60  ISO KEY
09  8       23  H             3D  F3            61  UNDO
0A  9       24  J             3E  F4            62  HELP
0B  0       25  K             3F  F5            63  KEYPAD (
0C  -       26  L             40  F6            64  KEYPAD /
0D  ==      27  ;             41  F7            65  KEYPAD *
0E  BS      28  '             42  F8            66  KEYPAD *
0F  TAB     29  `             43  F9            67  KEYPAD 7
10  Q       2A  (LEFT) SHIFT  44  F10           68  KEYPAD 8
11  W       2B  \	            45  { NOT USED }  69  KEYPAD 9
12  E       2C  Z             46  { NOT USED }  6A  KEYPAD 4
13  R       2D  X             47  HOME          6B  KEYPAD 5
14  T       2E  C             48  UP ARROW      6C  KEYPAD 6
15  Y       2F  V             49  { NOT USED }  6D  KEYPAD 1
16  U       30  B             4A  KEYPAD -      6E  KEYPAD 2
17  I       31  N             4B  LEFT ARROW    6F  KEYPAD 3
18  O       32  M             4C  { NOT USED }  70  KEYPAD 0
19  P       33  ,             4D  RIGHT ARROW   71  KEYPAD .
1A  [       34  .             4E  KEYPAD +      72  KEYPAD ENTER
*/

static u_char dr1_getb (offs)
u_int offs;
{
  u_char value=0xFF;
  u_char  ddr1=iram[DDR1];
  u_char  ddr3=iram[DDR3];
  u_char  ddr4=iram[DDR4];
  u_char  dr2=iram[P2];
  u_char  dr3=iram[P3];
  u_char  dr4=iram[P4];
  int dr1bit;
  int mask,scancode=-1;
//  ASSERT(offs==P1);
//  ASSERT(!ddr1); // strong
//  ASSERT(!(dr2&1)); // strong, asserts at reset?
   // We make DR1 bit by bit
  for(dr1bit=0;dr1bit<8;dr1bit++)
  {
    mask=1<<dr1bit; 
    // we only consider bits with a corresponding 0 in DDR1
    if(!(mask&ddr1) && !(dr2&1))
    {
      int column,mask2,found=0; // 'found' avoids being too slow
      // test DR3
      for(column=0;column<7&&!found;column++)
      {
        mask2=1<<(column+1);
        scancode=get_scancode(dr1bit,column);
        if(ST_Key_Down[scancode]
          &&  (dr3&mask2) // must be set (diode on)
          &&  (ddr3&mask2)
          )
          found++;
      }
      // test DR4
      for(column=7;column<15&&!found;column++)
      {
        mask2=1<<(column-7);
        scancode=get_scancode(dr1bit,column);
        if(ST_Key_Down[scancode]
          &&  (dr4&mask2) // must be set (diode on)
          &&  (ddr4&mask2)
          )
          found++;
      }
      if(found)
      {
        value&=~mask; // clear bit
        //TRACE("Read DR1 %X scancode %x\n",value,scancode);
      }
    }
  }
//  ASSERT(value);
  return value;
}
 
/*  DR2 ($03 DDR2: $01)
    Bit 0 : Joystick 1 pin 5, output for selecting the 74LS244 (see DR4)
    Bit 1 : Left mouse button or joystick 0 Fire button.
    Bit 2 : Right mouse button or joystick 1 Fire button.
    Bit 3 : Acia TxDATA (6301 receive)
    Bit 4 : Acia RxDATA (6301 transmit)

    The ROM may write either 1 or 0 on DR2.
    When we read, DDR2 is always 1.
    Button pressed -> bit cleared (not set)
    mousek 1 ->   value 2
    mouesk 2 ->   value 4
    mousek 3 ->   value 0
*/

static u_char dr2_getb (offs)
  u_int offs;
{
  u_char value;
#if !defined(NDEBUG)
//  u_char ddr2=iram[DDR2];
  //ASSERT(ddr2==1); // strong
#endif
  //ASSERT(offs==P2);
  value=0xFF; // note bits 5-7=111 in monochip mode, bits 3-4=serial lines
  if(mousek) // clear the correct bit (see above)
  {
    value=(mousek*2)%6;
//    TRACE("HD6301 handling mousek %x -> %x\n",mousek,value);
  }
  return value;
}

/*  DR4 ($07 DDR4: $05)

    This register is also used to read joysticks directions.
    For this you need to select the 74LS244 by setting the bit 0 of DR2 to 0,
    [in fact this bit should be set?]
    on output, then turn the DDR4 to all input (%00000000). 
    The four directions of each joystick are then readable on bits [0-3] and 
    [4-7].
    For keyboard handling, see DR1, DR3.

    Bit 0: XB & Up0
    Bit 1: XA & Down0
    Bit 2: YA & Left0
    Bit 3: YB & Right0
    Bit 4: Up1            NOT $10 = $EF
    Bit 5: Down1          NOT $20 = $DF
    Bit 6: Left1          NOT $40 = $BF
    Bit 7: Right1         NOT $80 = $7F
    
    XB, XA, YA, YB (mouse) are mapped on bits 4-7 if mouse is in port 1 
    (not emulated, no use)
    X# horizontal movement
    Y# vertical movement
*/

static u_char dr4_getb (offs)
  u_int offs;
{
  u_char value;
  u_char  ddr2=iram[DDR2];
  u_char  ddr4=iram[DDR4];
  u_char  dr2=iram[P2];
  int joy0mvt=0,joy1mvt=0;
//  ASSERT(offs==P4);
//  ASSERT(!ddr4); // strong
//  ASSERT(ddr2&1); // strong
 // ASSERT(!(dr2&1));
  value=0xFF;
/*  Mouse movements
    As the mouse ball rotates, two axes spin and cause the logical rotation
    of a 0011 bit sequence in the hardware, two bits going to the 
    registry when read. To emulate this, we rotate a $3 (0011) sequence and
    send the last bits to registry bits 0-1 for horizontal movement, 2-3
    for vertical movement. */
  update_mouse();

/*  Joystick movements
    Movement is signalled by cleared bits.
*/
  if(!ddr4 && (ddr2&1) && (dr2&1))
  {
    joy0mvt=stick[0]&0xF; // eliminate fire info
    joy1mvt=stick[1]&0xF;
    if(joy0mvt||joy1mvt)
    {
      value=0; // not always right (game mouse+joy?) but can't do better yet
      value|=joy0mvt|(joy1mvt<<4);
      value=~value;
      //TRACE("sticks %X 0 %x 1 %X\n",value,stick[0],stick[1]);
    }
  }  
  if (!IKBDConfig.Port0Joy)
    value=(value&(~0xF))|(mouse_x_counter&3)|((mouse_y_counter&3)<<2);
  //iram[offs]=value; //no! Froggies menu would take / and enter as well as 0
  return value;
}


static port_putb P_((u_int offs, u_char value));

static
port_putb (offs, value)
  u_int  offs;
  u_char value;
{
  /* Only change output ports - xor with DDR */
  ireg_putb (offs, value ^ ireg_getb (offs - 2));
}


/*
 *  Pointers to functions to be called for reading internal registers
 *  ST: notice the addition of dr#_getb function pointers
 */
u_char (*ireg_getb_func[NIREGS]) P_((u_int offs)) = {
/* 0x00 */
  0,          0,  dr1_getb ,  dr2_getb ,
  0,          0,          0,  dr4_getb ,
  tcsr_getb,  0,          0,          0,
  0,          0,          0,          0,
/* 0x10 */
  0,          trcsr_getb, rdr_getb,   0,
  0
};


/*
 *  Pointers to functions to be called for writing internal registers
*/
int (*ireg_putb_func[NIREGS]) P_((u_int offs, u_char val)) = {
/* 0x00 */
  0,          0,          port_putb,   port_putb,
  0,          0,          port_putb,   port_putb,
  tcsr_putb,  0,          0,           ocr_putb,
  ocr_putb,   0,          0,           0,
/* 0x10 */
  0,          trcsr_putb, 0,           tdr_putb,
  0
};

#undef P_
