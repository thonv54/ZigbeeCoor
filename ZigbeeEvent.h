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
#ifndef __ZIGBEEEVENT_H__
#define __ZIGBEEEVENT_H__

#include "app/framework/include/af.h"
#include "EventDataBuffer.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define Debug


typedef enum{
    NewBindUnbindRequest_step               = 0,
    ReceiverIeeeResponseFirst_step          = 1,
    ReceiverIeeeResponseSecond_step         = 2,
    ReceiverBindUnbindResponseFirst_step    = 3,
    ReceiverBindUnbindResponseSecond_step   = 4,
    
}BindUnbindEventStep_enum;

typedef enum {
    BindUnbindError = 1,
    BindUnbindSuccess = 0,
}BindUnbindStatus_enum;

typedef enum{
    EventIdNotContain = 0xFF,
}EventDulicateStatus_enum;

typedef struct {
    int16u PanId;
    int8u Channel;
}NwkFormEventData_str;

typedef struct{
    int8u BindingStep;
    int16u LastBindTimmer;
    int16u NwkAddr1;
    EmberEUI64 IeeeAddr1;
    int8u Endpoint1;
    int16u NwkAddr2;
    EmberEUI64 IeeeAddr2;
    int8u Endpoint2;
    int16u ClusterId;
    int8u Status;
}BindDeviceEventData_str;

typedef struct{
    int8u UnbindingStep;
    int16u LastUnbindTimmer;
    int16u NwkAddr1;
    EmberEUI64 IeeeAddr1;
    int8u Endpoint1;
    int16u NwkAddr2;
    EmberEUI64 IeeeAddr2;
    int8u Endpoint2;
    int16u ClusterId;
    int8u Status;
}UnbindDeviceEventData_str;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



extern NwkFormEventData_str NwkFormEventData;
extern BindDeviceEventData_str BindDeviceEventData;
extern UnbindDeviceEventData_str UnbindDeviceEventData;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

extern EmberEventControl NwkFormEventControl;
extern EmberEventControl BindDeviceEventControl;
extern EmberEventControl UnbindDeviceEventControl;

/******************************************************************************/
/*                            END OF FILE                                     */
/******************************************************************************/









#endif