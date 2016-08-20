

#include "app/framework/include/af.h"
//-------------------------Enum------------------------------------//


typedef enum TaskList{
    TaBlinkLed1 	= 			1,
    TaBlinkLed2 	= 			2,
    TaBlinkLed3 	= 			3,
    TaBlinkLed4		=			4,
    TaBlinkLedAll	=			5,
    TaButtonDetect 	= 			6,
    TaJoin        	=   			7,
    TaOnOffAttributeChange = 			8,
    TaResetMCU		=			9,
    TaUartSendDimLevel = 			10,
    TaMcTimerCalibrate = 			11,

}TaskList_enum;

//------------------------------Struct------------------------------//

typedef struct {
	int16u ID;
	int16u Length;
	int16u Inverval;
	int16u MaxTime;
	int16u TimeCnt;
	int16u BeforeTimer;
}TaskMcTimmerCalibrateData_str;
typedef struct {
	int16u ID;
	int16u Length;
	int16u MaxDimLevel;
	int16u StepTime;
	int16u BeforeDimLevel;
	int16u BeforeTimer;
}TaskUartSendDimLevelData_str;
typedef struct {
    int16u BlinkLedID;
    int16u Length;
    int16u DelayTime;
    int16u BlinkCnt;
    int16u MaxCnt;
    int16u BeforeTimer;
} TaskBlinkLedData_str;
typedef struct {
	int16u ID;
	int16u Length;
	int16u TimeCountDown;
	int16u BeforeTimer;
}TaskResetMCUData_str;

typedef struct {
    int16u ButtonDetectID;
    int16u Length;
    int16u StateBefore;
    int16u PressCnt;
    int16u PressOnTime;
	int16u ComboPressCnt;

}TaskButtonDetectData_str;

typedef struct {
    int16u ID;
    int16u Length;
    int16u TimeMax;
    int16u TimeCnt;
    int16u Cycle;
    int16u BeforeTimer;
}TaskJoinData_str;

typedef struct{
	int16u ID;
	int16u Length;
	int16u Endpoint;
	int16u Value;
}TaskOnOffAttributeChangeData_str;



