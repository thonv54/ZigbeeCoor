/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: Thonv $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-06-08 10:45:00 (Wen, 08 Jun 2016) $
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "macro.h"
#include "typedefs.h"

#include "app/framework/include/af.h"
#include "EventDataBuffer.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
u8_t g_byEventDataBuff[255] = { 0 };

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/


/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/


//----------------------Event Data Store Utility-----------------------------//

/**
 * @func   GetLastStoreIndex
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
u8_t GetLastStoreIndex(void_t) {
    u8_t byHead = 0;
    u8_t byLength = 0;
    while (g_byEventDataBuff[byHead] != 0) {
        if (g_byEventDataBuff[byHead] != 0) {
            byLength = g_byEventDataBuff[byHead + 1] + 2;
            byHead = byHead + byLength;
        }
    }
    return byHead;
}
/**
 * @func   DeleteEventDataByIndex
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void_t DeleteEventDataByIndex(u8_t byHead) {
    u8_t byLength = 0;

    byLength = g_byEventDataBuff[byHead + 1] + 2;
    MEMCOPY(&g_byEventDataBuff[byHead], &g_byEventDataBuff[byHead + byLength],
            (EventDataSize - byHead - byLength));
}

/**
 * @func   FindEventDataId
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
u8_t FindEventDataId(u8_t EventId) {
    u8_t byHead;
    u8_t CurrentEventId;

    byHead = 0;
    CurrentEventId = g_byEventDataBuff[0];
    while (CurrentEventId != 0) {

        if (EventId == CurrentEventId) {
            return byHead;
        }
        else {
            byHead = byHead + g_byEventDataBuff[byHead + 1] + 2;
            CurrentEventId = g_byEventDataBuff[byHead];
        }
    }
    return EventIdNotFound;
}
/**
 * @func   DeleteEventDataById
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
u8_t DeleteEventDataById(u8_t EventId) {
    u8_t EventIndex;
    EventIndex = FindEventDataId(EventId);
    if (EventIndex != EventIdNotFound) {
        DeleteEventDataByIndex(EventIndex);
        return EventClearOK;
    }
    else {
        return EventIdNotFound;
    }

}
/**
 * @func   StoreEventDataInBuffer
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

void_t StoreEventDataInBuffer(u8_t EventId, u8_t DatabyLength, u8_p Data) {
    u8_t LastStoreIndex = 0;
    LastStoreIndex = GetLastStoreIndex();
    g_byEventDataBuff[LastStoreIndex] = EventId;
    g_byEventDataBuff[LastStoreIndex + 1] = DatabyLength;
    MEMCOPY(&g_byEventDataBuff[LastStoreIndex + 2], Data, DatabyLength);
}
/**
 * @func   GetEventDataFromBuff
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

u8_t GetEventDataFromBuff(u8_t EventId, u8_p Ret) {
    u8_t byHead;
    u8_t byLength;

    byHead = FindEventDataId(EventId);
    byLength = g_byEventDataBuff[byHead + 1];
    if (byHead == EventIdNotFound) {
        return EventIdNotFound;
    }
    else {
        MEMCOPY(Ret, &g_byEventDataBuff[byHead + 2], byLength);
        return EventDataFound;
    }
}
