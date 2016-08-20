



#ifndef _TASKUTILITY_H
#define _TASKUTILITY_H


#include "app/framework/include/af.h"

//---------------------Global Var--------------//

//---------------------Define------------------//

#define TASKDATASIZE  64


//---------------------Enum--------------------//


typedef enum taskStatus{
    CompleteTask = 1,
    ContinuteTask = 2,
}taskStatusEnum;


typedef enum taskDulicateStatus{
    TaskOK = 0xFF,
}taskDulicateStatusEnum;

typedef enum taskClearStatus{
    TaskClearOK = 0xFF,
    TaskIdNotFound = 1,
}taskClearStatusEnum;


//-------------------Protocol---------------------//


unsigned char GetLastTask(void);
void TaskDelete (unsigned char head);
unsigned char FindTaskID(unsigned char TaskID);
unsigned char ClearTask(unsigned char taskID);

void CallResetMCU(int16u ResetTimeCountdown);
int8u TaskResetMCU(int16u *Data);


void TaskRun (void);

#endif




