/***************************************************************\
|                  Format Block Specification                   |
\************************************************************** /

Len	Offs	BlockName & Description
1 	0   	validador (0-1953)
720	1   	wall
720	721 	back
18	1441	doors1
238	1459	unknown1 (22/215?) (por convencion: empty para guardar info)
18	1697	doors2
238	1715	unknown2 (22/215?) (por convencion: empty para guardar info)
96	1953	links
64	2049	unknown3 (peligroso)
3 	2113	startPosition (1: screen; 2: module; 3: direction)
3 	2116	unknownSP (Dafault: triple 0) (por convencion: 1: comienzo de string info; 2 y 3: Debug Position)
1 	2117	startPositionUnknown (direction?)
24	2120	guardPosition
24	2144	guardDirection (00: right; FF: left)
1 	2168	unknown4/1 (por convencion: validator 1953-2306)
23	2169	unknown4/23 (por convencion: empty para guardar info)
24	2192	unknown5 (por convencion: empty para guardar info)
24	2216	guardSkill
24	2240	unknown6 (por convencion: empty para guardar info)
24	2264	guardColor
16	2288	unknown7 (por convencion: empty para guardar info)
2 	2304	0F 09
  	2306	New Level

/ **************************************************************\
|                          Inclusiones                          |
\***************************************************************/

#ifndef INCLUDED_MAPS_H
#define INCLUDED_MAPS_H

#include <stdio.h>

/***************************************************************\
|                  Definiciones del Precompilador               |
\***************************************************************/

#define MAPS_MAX_SCREENS    24
#define MAPS_MAX_LENGTH     49
#define MAPS_LINKMAP_CENTER 24

#define MAPS_DIRECTION_RIGHT 0x00
#define MAPS_DIRECTION_LEFT  0xFF

#define MAPS_BLOCK_SIZEOF_LEVEL    					2306
#define MAPS_BLOCK_OFFSET_LEVELS   					6

#define MAPS_BLOCK_OFFSET_VALIDATOR_WALL		0
#define MAPS_BLOCK_SIZEOF_VALIDATOR_WALL		1
#define MAPS_BLOCK_OFFSET_WALL							1
#define MAPS_BLOCK_SIZEOF_WALL							720
#define MAPS_BLOCK_SIZEOF_BACK							720
#define MAPS_BLOCK_OFFSET_BACK							721
#define MAPS_BLOCK_OFFSET_LINK							1953
#define MAPS_BLOCK_OFFSET_VALIDATOR_LINK		2168
#define MAPS_BLOCK_OFFSET_GUARD_DIRECTION 	2144
#define MAPS_BLOCK_OFFSET_GUARD_POSITION  	2120
#define MAPS_BLOCK_OFFSET_START_POSITION    2113
#define MAPS_BLOCK_OFFSET_GUARD_SKILL     	2216
#define MAPS_BLOCK_OFFSET_GUARD_COLOR     	2264
#define MAPS_BLOCK_OFFSET_UNKNOWN_1         1459
#define MAPS_BLOCK_SIZEOF_UNKNOWN_1         238
#define MAPS_BLOCK_OFFSET_UNKNOWN_2         1715
#define MAPS_BLOCK_SIZEOF_UNKNOWN_2         238
#define MAPS_BLOCK_OFFSET_UNKNOWN_4         2169
#define MAPS_BLOCK_SIZEOF_UNKNOWN_4         23
#define MAPS_BLOCK_SIZEOF_UNKNOWN_5         24
#define MAPS_BLOCK_OFFSET_UNKNOWN_5         2192
#define MAPS_BLOCK_SIZEOF_UNKNOWN_6         24
#define MAPS_BLOCK_OFFSET_UNKNOWN_6         2240
#define MAPS_BLOCK_SIZEOF_UNKNOWN_7         16
#define MAPS_BLOCK_OFFSET_UNKNOWN_7         2288
#define MAPS_BLOCK_SIZEOF_GATE_1            18
#define MAPS_BLOCK_SIZEOF_GATE              18
#define MAPS_BLOCK_OFFSET_GATE_1            1441
#define MAPS_BLOCK_SIZEOF_GATE_2            18
#define MAPS_BLOCK_OFFSET_GATE_2            1697
#define MAPS_BLOCK_SIZEOF_FILE              37031


/* Constantes copadas */
#define MAPS_sUp			2
#define MAPS_sDown		3
#define MAPS_sLeft		0
#define MAPS_sRight		1

#define T_EMPTY 0x00           /* 00000 free    */
#define T_FLOOR 0x01           /* 00001 free    */
#define T_SPIKES 0x02          /* 00010 spike   */
#define T_PILLAR 0x03          /* 00011 none    */
#define T_GATE 0x04            /* 00100 gate    */
#define T_BTN_STUCK 0x05       /* 00101 none    */
#define T_BTN_DROP 0x06        /* 00110 event   */
#define T_TAPESTRY 0x07        /* 00111 tapest  */
#define T_BP_BOTTOM 0x08       /* 01000 none    */
#define T_BP_TOP 0x09          /* 01001 none    */
#define T_POTION 0x0A          /* 01010 potion  */
#define T_LOOSE 0x0B           /* 01011 none    */
#define T_TAPESTRY_TOP 0x0C    /* 01100 ttop    */
#define T_MIRROR 0x0D          /* 01101 none    */
#define T_DEBRIS 0x0E          /* 01110 none    */
#define T_BTN_RAISE 0x0F       /* 01111 event   */
#define T_EXIT_LEFT 0x10       /* 10000 none    */
#define T_EXIT_RIGHT 0x11      /* 10001 none    */
#define T_CHOPPER 0x12         /* 10010 chomp   */
#define T_TORCH 0x13           /* 10011 none    */
#define T_WALL 0x14            /* 10100 wall    */
#define T_SKELETON 0x15        /* 10101 none    */
#define T_SWORD 0x16           /* 10110 none    */
#define T_BALCONY_LEFT 0x17    /* 10111 none    */
#define T_BALCONY_RIGHT 0x18   /* 11000 none    */
#define T_LATTICE_PILLAR 0x19  /* 11001 none    */
#define T_LATTICE_SUPPORT 0x1A /* 11010 none    */
#define T_LATTICE_SMALL 0x1B   /* 11011 none    */
#define T_LATTICE_LEFT 0x1C    /* 11100 none    */
#define T_LATTICE_RIGHT 0x1D   /* 11101 none    */
#define T_TORCH_DEBRIS 0x1E    /* 11110 none    */
#define T_NULL 0x1F            /* 11111 none    */
/*
  none   0x00 This value is used allways for this group
  free   0x00 +Nothing -Blue line
  free   0x01 +Spot1   -No blue line
  free   0x02 +Spot2   -Diamond
  free   0x03 Window
  free   0xFF +Spot3   -Blue line?
  spike  0x00 Normal (allows animation)
  spike  0x01 Barely Out
  spike  0x02 Half Out
  spike  0x03 Fully Out
  spike  0x04 Fully Out
  spike  0x05 Out?
  spike  0x06 Out?
  spike  0x07 Half Out?
  spike  0x08 Barely Out?
  spike  0x09 Disabled
  gate   0x00 Closed
  gate   0x01 Open
  tapest 0x00 -With Lattice
  tapest 0x01 -Alternative Design
  tapest 0x02 -Normal
  tapest 0x03 -Black
  potion 0x00 Empty
  potion 0x01 Health point
  potion 0x02 Life
  potion 0x03 Feather Fall
  potion 0x04 Invert
  potion 0x05 Poison
  potion 0x06 Open
  ttop   0x00 -With lattice
  ttop   0x01 -Alernative design
  ttop   0x02 -Normal
  ttop   0x03 -Black
  ttop   0x04 -Black
  ttop   0x05 -With alternative design and bottom
  ttop   0x06 -With bottom
  ttop   0x07 -With window
  chomp  0x00 Normal
  chomp  0x01 Half Open
  chomp  0x02 Closed
  chomp  0x03 Partially Open
  chomp  0x04 Extra Open
  chomp  0x05 Stuck Open
  wall   0x00 +Normal  -Blue line
  wall   0x01 +Normal  -No Blue line
*/

#define MAP_B_NONE 0
#define MAP_F_WALL T_WALL
#define MAP_F_FREE T_EMPTY

#endif
