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

/* Definicion de cosas: */

/* WALLS */
#define MAPS_OBJ_ANT_1 0x13
#define MAPS_OBJ_ANT_2 0x33
#define MAPS_OBJ_BAL_1 0x01
#define MAPS_OBJ_BAL_2 0x11
#define MAPS_OBJ_BAL_3 0x21
#define MAPS_OBJ_BAL_D 0x26
#define MAPS_OBJ_BAL_U 0x2f
#define MAPS_OBJ_BFLO  0x0b
#define MAPS_OBJ_BRA_1 0x1e
#define MAPS_OBJ_BRA_2 0x3e
#define MAPS_OBJ_BRT_1 0x0e
#define MAPS_OBJ_BRT_2 0x2e
#define MAPS_OBJ_CDB_D 0x28
#define MAPS_OBJ_CDB_U 0x09
#define MAPS_OBJ_COL_1 0x03
#define MAPS_OBJ_COL_2 0x23
#define MAPS_OBJ_ESP_1 0x16
#define MAPS_OBJ_ESP_2 0x36
#define MAPS_OBJ_ESQU  0x15
#define MAPS_OBJ_FREE  0x00
#define MAPS_OBJ_PAR_1 0x14
#define MAPS_OBJ_PAR_2 0x34
#define MAPS_OBJ_PIN_1 0x02
#define MAPS_OBJ_PIN_2 0x22
#define MAPS_OBJ_PLV_L 0x30
#define MAPS_OBJ_PLV_R 0x31
#define MAPS_OBJ_POS_1 0x0a
#define MAPS_OBJ_POS_2 0x2a
#define MAPS_OBJ_PUE_1 0x24
#define MAPS_OBJ_PUE_U 0x0c
#define MAPS_OBJ_SIER  0x32
#define MAPS_OBJ_VEN_L 0x37
#define MAPS_OBJ_VEN_R 0x38

/* BACKS */
#define MAPS_BCK_VEN_R 0x00
#define MAPS_BCK_BRK_1 0x01
#define MAPS_BCK_BRK_2 0x02
#define MAPS_BCK_BRK_3 0xFF
#define MAPS_BCK_VEN_1 0x03
#define MAPS_BCK_PUE_U 0x01
#define MAPS_BCK_PUE_D 0x02
#define MAPS_BCK_POS_G 0x01
#define MAPS_BCK_POS_C 0x02
#define MAPS_BCK_POS_W 0x03
#define MAPS_BCK_POS_I 0x04
#define MAPS_BCK_POS_A 0x05
#define MAPS_BCK_ALF_1 0x00
#define MAPS_BCK_ALF_2 0x01
#define MAPS_BCK_ALF_3 0x02
#define MAPS_BCK_PIN_1 0x00
#define MAPS_BCK_PIN_2 0x01
#define MAPS_BCK_PIN_3 0x02
#define MAPS_BCK_PIN_4 0x03

#endif
