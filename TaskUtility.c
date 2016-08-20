


#include "app/framework/include/af.h"
#include "macro.h"
#include "typedefs.h"
#include "TaskID.h"
#include "halDelay.h"


#include "TaskUtility.h"



int16u TaskData[64];





//------------------------------Task Utility-------------------------//
unsigned char GetLastTask(void){
    unsigned char Head = 0;
    unsigned char Length = 0;
    while(TaskData[Head] != 0){
        if(TaskData[Head] != 0){
            Length = TaskData[Head+1]+2;
            Head = Head+Length;
        }
    }
    return Head;
}
void TaskDelete (unsigned char head){
    unsigned char TaskDataLength = 0;
    TaskDataLength = TaskData[head+1]+2;
    MEMCOPY(&TaskData[head], &TaskData[head + TaskDataLength], (TASKDATASIZE - head - TaskDataLength)*2);
}
unsigned char FindTaskID(unsigned char TaskID){
    unsigned char Head;
    unsigned char CurrentTaskID;

    Head = 0;
    CurrentTaskID = TaskData[0];
    while(CurrentTaskID !=0){

        if(TaskID == CurrentTaskID){
            return Head;
        }
        else{
             Head = Head + TaskData[Head+1]+2;
             CurrentTaskID = TaskData[Head];
        }
    }
    return TaskOK;
}
unsigned char ClearTask(unsigned char taskID){
	unsigned char taskPos;
	taskPos = FindTaskID(taskID);
	if(taskPos != TaskOK){
		 TaskDelete(taskPos);
		 return TaskClearOK;
	}
	else{
		return TaskIdNotFound;
	}

}

//-----------------------------TaskRun-------------------------------//
void TaskRun (void){

    unsigned char TaskID;
    unsigned char Head;
    unsigned char TaskStatus;

    Head = 0;
    TaskID = TaskData[Head];
    while (TaskID !=0){
        switch (TaskID){
            case TaBlinkLed1:

                break;
            case TaBlinkLed2:

                break;
            case TaBlinkLed3:

                break;
            case TaBlinkLed4:

                break;
            case TaBlinkLedAll:

                break;
            case TaJoin:

                  break;
            case TaOnOffAttributeChange:

                  break;
            case TaResetMCU:
                  TaskStatus = TaskResetMCU(&TaskData[Head]);
                  break;
                    
                    
            default:
                break;
        }
        if(TaskStatus != CompleteTask){
            Head = Head + TaskData[Head + 1]+2;
            TaskID = TaskData[Head];
        }
        else{
            TaskDelete(Head);
            Head = 0;
            TaskID = TaskData[0];
        }
    }
}
//-----------------------------TaskCmd-------------------------------//






//-----------------------------ResetMCU------------------------------//
void CallResetMCU(int16u ResetTimeCountdown){
	TaskResetMCUData_str ResetMCUData;
	ResetMCUData.ID = TaResetMCU;
	ResetMCUData.Length = 2;
	ResetMCUData.TimeCountDown = ResetTimeCountdown;
	ResetMCUData.BeforeTimer = (int16u)halCommonGetInt32uMillisecondTick();

	MEMCOPY(&TaskData[GetLastTask()],&ResetMCUData, sizeof(TaskResetMCUData_str));
}
int8u TaskResetMCU(int16u *Data){
	TaskResetMCUData_str ResetMCUData;


	MEMCOPY(&ResetMCUData,Data, sizeof(TaskResetMCUData_str));

    if(CommonGetDurationTime(ResetMCUData.BeforeTimer) > ResetMCUData.TimeCountDown){
		halReboot();
		return CompleteTask;
	}
	else{
		return ContinuteTask;
	}
}
//-------------------**----------********---------**-------------------//







//-----------------------------************----------------------------//

//-----------------------------***END-FILE***--------------------------//
//-----------------------------************--------------------------//













