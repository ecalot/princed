#ifndef _STATES_H_
#define _STATES_H_

/* Structs */

#include "types.h" /* tMap */
#include "input.h" /* tKey */

typedef enum {esLast=0,esKeyUp,esKeyDown,esKeyForward,esKeyBack,esKeyShift,esMapUp,esMapDown,esMapForward,esMapBack,esMapOn,esForwardTileNearerThan,esForwardTileFartherThan,esScreenUp,esScreenDown,esScreenLeft,esScreenRight,esInScreen,esInLevel,esForwardChangeToScreen,esInFloorTop,esInfloorMiddle,esInfloorBottom}tsConditionType;

								
typedef enum {esDangerous,esNone,esWalk,esPotion,esSword,esMirror,esNotApplicable=0}tsTileType;
typedef enum {esRelative,esForwardTile,esRelativeTurn}tsMoveType; /*  */

typedef struct {
	tsConditionType type;
	short           argument;
} tsCondition;

typedef struct {
	short        conditionId; /* Id of the first condition to be evaluated */
	tsMoveType   moveType; /* absolute or relative */
	char         moveOffset; /* amount of moving units the kid will move depending on the offset type */
	short        nextStateId; /* Id of the first action of the next state */
	short        animStart; /* Index of the first frame in the animation array */
	short        animSize; /* number of frames to be shown */
} tsAction;

#define stateGetImage(a) (((a).action.image))

/* public functions interface */

/* This function should return the frame flags
 * and actions to be performed by this call */
short stateUpdate(tKey* key, tKid* kid,tRoom* room);

/* Create a State depending on the level */
tState createState(int level);

#include "states_conf.h"

#define STATES_CONDRESULT_FALSE 2
#define STATES_CONDRESULT_END 1
#define STATES_CONDRESULT_TRUE 0

#endif

