
#ifndef __EVENTDATABUFFER_H__
#define __EVENTDATABUFFER_H__



#define Debug

#include "app/framework/include/af.h"



typedef enum EventClearStatus{
    EventClearOK = 1,
    EventIdNotFound = 0xFF,
    EventDataFound = 2,
}EventClearStatusEnum;


#define EventDataSize  255
extern int8u EventDataBuff[255];

int8u GetLastStoreIndex(void);
void DeleteEventDataByIndex(int8u Head);
int8u DeleteEventDataById(int8u EventId);
int8u FindEventDataId(int8u EventId);
void StoreEventDataInBuffer(int8u EventId, int8u DataLength, int8u* Data);















#endif