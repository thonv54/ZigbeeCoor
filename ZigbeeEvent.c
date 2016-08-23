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

NwkFormEventData_str NwkFormEventData;  // Event Data Struct


/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void NwkFormEventFunction(void); // Event function forward declaration


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
 * @param Length   Ver.: always
 */
boolean emberAfPreZDOMessageReceivedCallback(EmberNodeId emberNodeId,
        EmberApsFrame* apsFrame, int8u* message, int16u Length) {
    if (apsFrame->clusterId == ACTIVE_ENDPOINTS_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,ACTIVE_ENDPOINTS_RESPONSE,Length,message);
        return TRUE;
    }
    else if (apsFrame->clusterId == SIMPLE_DESCRIPTOR_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,SIMPLE_DESCRIPTOR_RESPONSE,Length,message);
        return TRUE;
    }
    else if (apsFrame->clusterId == NETWORK_ADDRESS_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,NETWORK_ADDRESS_RESPONSE,Length,message);
        return TRUE;
    }
    else if (apsFrame->clusterId == IEEE_ADDRESS_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,IEEE_ADDRESS_RESPONSE,Length,message);
        return TRUE;
    }
    else if (apsFrame->clusterId == END_DEVICE_ANNOUNCE) {
        UartSendZdoCmdResponse(emberNodeId,END_DEVICE_ANNOUNCE,Length,message);

        return TRUE;
    }
    else if (apsFrame->clusterId == END_DEVICE_BIND_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,END_DEVICE_BIND_RESPONSE,Length,message);
        return TRUE;
    }
    else if (apsFrame->clusterId == BIND_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,BIND_RESPONSE,Length,message);

        return TRUE;
    }
    else if (apsFrame->clusterId == UNBIND_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,UNBIND_RESPONSE,Length,message);

        return TRUE;
    }
    else if (apsFrame->clusterId == LEAVE_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,LEAVE_RESPONSE,Length,message);

        return TRUE;
    }
    else if (apsFrame->clusterId == NODE_DESCRIPTOR_RESPONSE) {
        UartSendZdoCmdResponse(emberNodeId,NODE_DESCRIPTOR_RESPONSE,Length,message);
        return TRUE;
    }

    return false;
}


/** @brief Pre Command Received
 *
 * This callback is the second in the Application Framework's message processing
 * chain. At this point in the processing of incoming over-the-air messages, the
 * application has determined that the incoming message is a ZCL command. It
 * parses enough of the message to populate an EmberAfClusterCommand struct. The
 * Application Framework defines this struct value in a local scope to the
 * command processing but also makes it available through a global pointer
 * called emberAfCurrentCommand, in app/framework/util/util.c. When command
 * processing is complete, this pointer is cleared.
 *
 * @param cmd   Ver.: always
 */
bool emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd)
{
    int16u NwkAddr = cmd-> source;
    int8u Endpoint = cmd-> apsFrame ->sourceEndpoint;
    int16u ClusterId  = cmd ->apsFrame -> clusterId;
    if(cmd -> clusterSpecific == ZCL_CLUSTER_SPECIFIC_COMMAND){
        int8u CmdPayloadLength = cmd -> bufLen - 2;
        int8u *CmdPayload = (int8u*)&(cmd -> buffer[2]);
        UartSendZclClusterCmdResponse(NwkAddr,Endpoint,ClusterId,CmdPayloadLength,CmdPayload);
    }
    else if (cmd -> clusterSpecific == ZCL_GLOBAL_COMMAND){
        int8u CmdPayloadLength = cmd -> bufLen - 3;
        int8u GeneralCmd = cmd->buffer[2];
        int8u *CmdPayload = (int8u*)&(cmd ->buffer[3]);
        UartSendZclGlobalCmdResponse(NwkAddr,Endpoint,ClusterId,GeneralCmd,CmdPayloadLength,CmdPayload);
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






