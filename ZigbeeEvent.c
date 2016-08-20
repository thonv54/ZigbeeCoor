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

#include "halDelay.h"
#include "app/framework/include/af.h"
#include "app/util/common/form-and-join.h"
#include "Uart_Receiver.h"
#include "Uart_Transmitter.h"
#include "Uart_CmdExec.h"
#include "Uart_CmdEnum.h"
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "ZigbeeEvent.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
EmberEventControl NwkFormEventControl; // Event control struct declaration
EmberEventControl BindDeviceEventControl;
EmberEventControl UnbindDeviceEventControl;

NwkFormEventData_str NwkFormEventData;  // Event Data Struct
BindDeviceEventData_str BindDeviceEventData;
UnbindDeviceEventData_str UnbindDeviceEventData;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void NwkFormEventFunction(void); // Event function forward declaration
void BindDeviceEventFunction(void);

//-------------------------------------Fuction---------------------------------//
/** @brief Pre ZDO Message Received
 *
 * This function passes the application an incoming ZDO message and gives the
 * appictation the opportunity to handle it. By default, this callback returns
 * FALSE indicating that the incoming ZDO message has not been handled and
 * should be handled by the Application Framework.
 *
 * @param emberNodeId   Ver.: always
 * @param apsFrame   Ver.: always
 * @param message   Ver.: always
 * @param byLength   Ver.: always
 */
boolean emberAfPreZDOMessageReceivedCallback(EmberNodeId emberNodeId,
        EmberApsFrame* apsFrame, int8u* message, int16u byLength) {
    if (apsFrame->clusterId == ACTIVE_ENDPOINTS_RESPONSE) {
        int16u wNwkAddr;
        int8u byActiveEndpointCount;
        int8u* byEndpointList;
        wNwkAddr = MERGE16(message[3], message[2]);
        byActiveEndpointCount = message[4];
        byEndpointList = &message[5];
        UartSendActiveEndpoint(wNwkAddr, byActiveEndpointCount, byEndpointList);
        return TRUE;
    }
    else if (apsFrame->clusterId == SIMPLE_DESCRIPTOR_RESPONSE) {
        int16u wwNwkAddr;
        int8u byEndpoint;
        int16u wProfileId;
        int16u wDeviceId;
        int8u byInputClusterCount;
        int8u byInputClusterList[64];
        int8u byOutputClusterCount;
        int8u byOutputClusterList[64];

        wwNwkAddr = MERGE16(message[3], message[2]);
        byEndpoint = message[5];
        wProfileId = MERGE16(message[7], message[6]);
        wDeviceId = MERGE16(message[9], message[8]);
        byInputClusterCount = message[11];
        convertClustesIdsToTxBuff(&message[12], byInputClusterList,
                byInputClusterCount);
        byOutputClusterCount = message[12 + 2 * byInputClusterCount];
        convertClustesIdsToTxBuff(&message[13 + 2 * byInputClusterCount],
                byOutputClusterList, byOutputClusterCount);

        UartSendSimpleDescription(wwNwkAddr, byEndpoint, wProfileId, wDeviceId,
                byInputClusterCount, byInputClusterList, byOutputClusterCount,
                byOutputClusterList);
        return TRUE;
    }
    else if (apsFrame->clusterId == IEEE_ADDRESS_RESPONSE) {
        return TRUE;
    }
    else if (apsFrame->clusterId == END_DEVICE_ANNOUNCE) {
        int16u wNwkAddr;
        EmberEUI64 byMac;
        int8u byCapInfo;
        wNwkAddr = MERGE16(message[2], message[1]);
        memcpy(byMac, &message[3], 8);
        byCapInfo = message[12];
        SwapEndiannessEUI64(byMac);
        UartSendDeviceJoinNwk(wNwkAddr, byMac, byCapInfo);
        return TRUE;
    }
    else if (apsFrame->clusterId == END_DEVICE_BIND_RESPONSE) {
        return TRUE;
    }
    else if (apsFrame->clusterId == BIND_RESPONSE) {
        BindDeviceEventData.Status = message[1];
        if (BindDeviceEventData.BindingStep
                != ReceiverBindUnbindResponseFirst_step) {
            BindDeviceEventData.BindingStep =
                    ReceiverBindUnbindResponseFirst_step;
            emberEventControlSetActive(BindDeviceEventControl);
            return TRUE;
        }
        else if (BindDeviceEventData.BindingStep
                == ReceiverBindUnbindResponseFirst_step) {
            BindDeviceEventData.BindingStep =
                    ReceiverBindUnbindResponseSecond_step;
            emberEventControlSetActive(BindDeviceEventControl);
            return TRUE;
        }
        return TRUE;
    }
    else if (apsFrame->clusterId == UNBIND_RESPONSE) {
        BindDeviceEventData.Status = message[1];
        if (UnbindDeviceEventData.UnbindingStep
                != ReceiverBindUnbindResponseFirst_step) {
            UnbindDeviceEventData.UnbindingStep =
                    ReceiverBindUnbindResponseFirst_step;
            emberEventControlSetActive(UnbindDeviceEventControl);
            return TRUE;
        }
        else if (UnbindDeviceEventData.UnbindingStep
                == ReceiverBindUnbindResponseFirst_step) {
            UnbindDeviceEventData.UnbindingStep =
                    ReceiverBindUnbindResponseSecond_step;
            emberEventControlSetActive(UnbindDeviceEventControl);
            return TRUE;
        }
        return TRUE;
    }
    else if (apsFrame->clusterId == LEAVE_RESPONSE) {
        if (message[1] == EMBER_SUCCESS) {
            UartSendLeaveNwkInfo(apsFrame->sourceEndpoint);
        }
        return TRUE;
    }
    else if (apsFrame->clusterId == NODE_DESCRIPTOR_RESPONSE) {
        int16u wNwkAddr;
        int8u byNodeDescription[13];

        wNwkAddr = MERGE16(message[3], message[2]);
        memcpy(byNodeDescription, &message[4], 13);
        UartSendNodeDescription(wNwkAddr, byNodeDescription);
        return TRUE;
    }

    return false;
}

/** @brief Read Attributes Response
 *
 * This function is called by the application framework when a Read Attributes
 * Response command is received from an external device.  The application should
 * return true if the message was processed or false if it was not.
 *
 * @param clusterId The cluster identifier of this response.  Ver.: always
 * @param buffer Buffer containing the list of read attribute byStatus records. 
 * Ver.: always
 * @param bufLen The byLength in bytes of the list.  Ver.: always
 */
bool emberAfReadAttributesResponseCallback(EmberNodeId emberNodeId,
        int8u sourceEndpoint, EmberAfClusterId clusterId, int8u *buffer,
        int16u bufLen) {
    int16u wNwkAddr = emberNodeId;
    int8u byEndpoint = sourceEndpoint;
    int16u wClusterId = clusterId;
    int16u wAttributeId = MERGE16(buffer[0], buffer[1]);
    int8u byStatus = buffer[2];
    int8u byDataType = buffer[3];
    int8u byValuebyLength = (int8u)(bufLen - 4);
    int8u* byValue = &buffer[4];
    if (byStatus == EMBER_SUCCESS) {
#ifdef Debug
        emberSerialPrintf(APP_SERIAL,"Read Attribute Success \n\r");
        emberSerialPrintf(APP_SERIAL,"\n\r");
#endif
        UartSendDeviceAttributeInfo(wNwkAddr, byEndpoint, wClusterId,
                wAttributeId, byDataType, byValuebyLength, byValue);
    }
    else {
#ifdef Debug
        emberSerialPrintf(APP_SERIAL,"Read Attribute Error \n\r");
        emberSerialPrintf(APP_SERIAL,"\n\r");
#endif
    }
    return false;
}
/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void NwkFormEventFunction(void) {
    EmberStatus byStatus;
    EmberNodeType byCurrentNodeType;
    EmberNetworkStatus byNetworkStatus;
    //Tao mang
    byNetworkStatus = emberAfNetworkState();

    emberAfGetNodeType(&byCurrentNodeType);

    switch (byNetworkStatus) {
    case EMBER_NO_NETWORK:
        emberEventControlSetActive(NwkFormEventControl);
        if (emberFormAndJoinIsScanning() == 0) {

            EmberNetworkParameters networkParams;

            MEMSET(&networkParams, 0, sizeof(EmberNetworkParameters));
            emberAfGetFormAndJoinExtendedPanIdCallback(
                    networkParams.extendedPanId);
            networkParams.radioChannel = NwkFormEventData.Channel;
            networkParams.radioTxPower = 16;
            networkParams.panId = NwkFormEventData.PanId;
            byStatus = emberAfFormNetwork(&networkParams);
        }
        break;
    case EMBER_JOINED_NETWORK:
        if (byCurrentNodeType != EMBER_COORDINATOR) {
            byStatus = emberLeaveNetwork();
            emberEventControlSetActive(NwkFormEventControl);
        }
        else {
            EmberNetworkParameters networkParams;
            EmberNodeType byNodeType;
            byStatus = emberAfGetNetworkParameters(&byNodeType, &networkParams);
            SwapEndiannessEUI64(networkParams.extendedPanId);
            UartSendNwkInfo(networkParams.extendedPanId, networkParams.panId,
                    networkParams.radioChannel);
            emberEventControlSetInactive(NwkFormEventControl);
            byStatus = DeleteEventDataById(NwkFormEventControl.taskid);
#ifdef Debug
            emberSerialPrintf(APP_SERIAL,"      Forming Done");
            emberSerialPrintf(APP_SERIAL,"      Pan ID: 0x%2x \n\r",networkParams.panId);
            emberSerialPrintf(APP_SERIAL,"      Channel: %u \n\r",networkParams.radioChannel);

            emberSerialPrintf(APP_SERIAL,"\n\r");
#endif //Debug
        }
        break;
    default:
        emberEventControlSetActive(NwkFormEventControl);
        break;
    }
}

/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void BindEventServiceDiscoveryCallback(
        const EmberAfServiceDiscoveryResult* result) {
    if (result->status
            == EMBER_AF_UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE) {
        if (BindDeviceEventData.BindingStep == NewBindUnbindRequest_step) {
            BindDeviceEventData.BindingStep = ReceiverIeeeResponseFirst_step;
            if (result->zdoRequestClusterId == IEEE_ADDRESS_REQUEST) {
                memcpy(BindDeviceEventData.IeeeAddr1,
                        (uint8_t*) (result->responseData), sizeof(EmberEUI64));
                emberEventControlSetActive(BindDeviceEventControl);
            }
        }
        else if (BindDeviceEventData.BindingStep
                == ReceiverIeeeResponseFirst_step) {
            BindDeviceEventData.BindingStep = ReceiverIeeeResponseSecond_step;
            if (result->zdoRequestClusterId == IEEE_ADDRESS_REQUEST) {
                memcpy(BindDeviceEventData.IeeeAddr2,
                        (uint8_t*) (result->responseData), sizeof(EmberEUI64));
                emberEventControlSetActive(BindDeviceEventControl);
            }
        }
    }
}
/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void UnbindEventServiceDiscoveryCallback(
        const EmberAfServiceDiscoveryResult* result) {
    if (result->status
            == EMBER_AF_UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE) {
        if (UnbindDeviceEventData.UnbindingStep == NewBindUnbindRequest_step) {
            UnbindDeviceEventData.UnbindingStep =
                    ReceiverIeeeResponseFirst_step;
            if (result->zdoRequestClusterId == IEEE_ADDRESS_REQUEST) {
                memcpy(UnbindDeviceEventData.IeeeAddr1,
                        (uint8_t*) (result->responseData), sizeof(EmberEUI64));
                emberEventControlSetActive(UnbindDeviceEventControl);
            }
        }
        else if (UnbindDeviceEventData.UnbindingStep
                == ReceiverIeeeResponseFirst_step) {
            UnbindDeviceEventData.UnbindingStep =
                    ReceiverIeeeResponseSecond_step;
            if (result->zdoRequestClusterId == IEEE_ADDRESS_REQUEST) {
                memcpy(UnbindDeviceEventData.IeeeAddr2,
                        (uint8_t*) (result->responseData), sizeof(EmberEUI64));
                emberEventControlSetActive(UnbindDeviceEventControl);
            }
        }
    }
}
/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

void BindDeviceEventFunction(void) {
    if (CommonGetDurationTime(BindDeviceEventData.LastBindTimmer) > _500ms) {
        BindDeviceEventData.BindingStep = NewBindUnbindRequest_step;
    }
    BindDeviceEventData.LastBindTimmer =
            (int16u) halCommonGetInt32uMillisecondTick();
    switch (BindDeviceEventData.BindingStep) {
    case NewBindUnbindRequest_step:
        //get EUI64 of nwkaddr1
        emberAfFindIeeeAddress(BindDeviceEventData.NwkAddr1,
                BindEventServiceDiscoveryCallback);
        emberEventControlSetInactive(BindDeviceEventControl);
        break;

    case ReceiverIeeeResponseFirst_step:
        //get EUI64 of nwkadd2
        emberAfFindIeeeAddress(BindDeviceEventData.NwkAddr2,
                BindEventServiceDiscoveryCallback);
        emberEventControlSetInactive(BindDeviceEventControl);
        break;

    case ReceiverIeeeResponseSecond_step:
        //send bind to nwkadd1:
        emberBindRequest(
                BindDeviceEventData.NwkAddr1, // who gets the bind req
                BindDeviceEventData.IeeeAddr1,      // source eui IN the binding
                BindDeviceEventData.Endpoint1, BindDeviceEventData.ClusterId,
                UNICAST_BINDING, // binding type
                BindDeviceEventData.IeeeAddr2, // destination eui IN the binding
                0,               // groupId for new binding
                BindDeviceEventData.Endpoint2, EMBER_AF_DEFAULT_APS_OPTIONS);
        //send bind to nwkadd2:
        emberBindRequest(
                BindDeviceEventData.NwkAddr2, // who gets the bind req
                BindDeviceEventData.IeeeAddr2,      // source eui IN the binding
                BindDeviceEventData.Endpoint2, BindDeviceEventData.ClusterId,
                UNICAST_BINDING, // binding type
                BindDeviceEventData.IeeeAddr1, // destination eui IN the binding
                0,               // groupId for new binding
                BindDeviceEventData.Endpoint1, EMBER_AF_DEFAULT_APS_OPTIONS);
        emberEventControlSetInactive(BindDeviceEventControl);
        break;

    case ReceiverBindUnbindResponseFirst_step:
        emberEventControlSetInactive(BindDeviceEventControl);
        break;

    case ReceiverBindUnbindResponseSecond_step:
        BindDeviceEventData.BindingStep = NewBindUnbindRequest_step;
        //inactive event
#ifdef Debug
        emberSerialPrintf(APP_SERIAL,"      HC Bind Device DONE \n\r");
        emberSerialPrintf(APP_SERIAL,"      NwkAddr1: 0x%2x \n\r",BindDeviceEventData.NwkAddr1);
        emberSerialPrintf(APP_SERIAL,"      NwkAddr2: 0x%2x \n\r",BindDeviceEventData.NwkAddr2);
        emberSerialPrintf(APP_SERIAL,"\n\r");
#endif // Debug     
        UartSendBindUnbindDevice(eBindCmd, BindDeviceEventData.NwkAddr1,
                BindDeviceEventData.NwkAddr2, BindDeviceEventData.Endpoint1,
                BindDeviceEventData.Endpoint2, BindDeviceEventData.ClusterId,
                BindDeviceEventData.Status);
        emberEventControlSetInactive(BindDeviceEventControl);
        break;

    default:
        break;
    }
}

void UnbindDeviceEventFunction(void) {
    if (CommonGetDurationTime(UnbindDeviceEventData.LastUnbindTimmer)
            > _500ms) {
        UnbindDeviceEventData.UnbindingStep = NewBindUnbindRequest_step;
    }
    UnbindDeviceEventData.LastUnbindTimmer =
            (int16u) halCommonGetInt32uMillisecondTick();
    switch (UnbindDeviceEventData.UnbindingStep) {
    case NewBindUnbindRequest_step:
        //get EUI64 of nwkaddr1
        emberAfFindIeeeAddress(UnbindDeviceEventData.NwkAddr1,
                UnbindEventServiceDiscoveryCallback);
        emberEventControlSetInactive(UnbindDeviceEventControl);
        break;

    case ReceiverIeeeResponseFirst_step:
        //get EUI64 of nwkadd2
        emberAfFindIeeeAddress(UnbindDeviceEventData.NwkAddr2,
                UnbindEventServiceDiscoveryCallback);
        emberEventControlSetInactive(UnbindDeviceEventControl);
        break;

    case ReceiverIeeeResponseSecond_step:
        //send Unbind to nwkadd1:
        emberUnbindRequest(
                UnbindDeviceEventData.NwkAddr1, // who gets the Unbind req
                UnbindDeviceEventData.IeeeAddr1,  // source eui IN the Unbinding
                UnbindDeviceEventData.Endpoint1,
                UnbindDeviceEventData.ClusterId, UNICAST_BINDING, // Unbinding type
                UnbindDeviceEventData.IeeeAddr2, // destination eui IN the Unbinding
                0,               // groupId for new Unbinding
                UnbindDeviceEventData.Endpoint2, EMBER_AF_DEFAULT_APS_OPTIONS);
        //send Unbind to nwkadd2:
        emberUnbindRequest(
                UnbindDeviceEventData.NwkAddr2, // who gets the Unbind req
                UnbindDeviceEventData.IeeeAddr2,  // source eui IN the Unbinding
                UnbindDeviceEventData.Endpoint2,
                UnbindDeviceEventData.ClusterId, UNICAST_BINDING, // Unbinding type
                UnbindDeviceEventData.IeeeAddr1, // destination eui IN the Unbinding
                0,               // groupId for new Unbinding
                UnbindDeviceEventData.Endpoint1, EMBER_AF_DEFAULT_APS_OPTIONS);
        emberEventControlSetInactive(UnbindDeviceEventControl);
        break;

    case ReceiverBindUnbindResponseFirst_step:
        emberEventControlSetInactive(UnbindDeviceEventControl);
        break;

    case ReceiverBindUnbindResponseSecond_step:
        UnbindDeviceEventData.UnbindingStep = NewBindUnbindRequest_step;
        //inactive event
#ifdef Debug
        emberSerialPrintf(APP_SERIAL,"      HC Unbind Device DONE \n\r");
        emberSerialPrintf(APP_SERIAL,"      NwkAddr1: 0x%2x \n\r",UnbindDeviceEventData.NwkAddr1);
        emberSerialPrintf(APP_SERIAL,"      NwkAddr2: 0x%2x \n\r",UnbindDeviceEventData.NwkAddr2);
        emberSerialPrintf(APP_SERIAL,"\n\r");
#endif    // Debug  
        UartSendBindUnbindDevice(eUnbindCmd, UnbindDeviceEventData.NwkAddr1,
                UnbindDeviceEventData.NwkAddr2, UnbindDeviceEventData.Endpoint1,
                UnbindDeviceEventData.Endpoint2,
                UnbindDeviceEventData.ClusterId, UnbindDeviceEventData.Status);

        emberEventControlSetInactive(UnbindDeviceEventControl);
        break;

    default:
        break;
    }
}

