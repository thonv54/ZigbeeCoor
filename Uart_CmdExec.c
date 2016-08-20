
/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: thonv $
 * Revision:         $Revision: 1.0.0.0 $
 * Last Changed:     $Date: 2016-05-20 10:20:00 (Fri, 20 May 2016) $
 *
 ******************************************************************************/

#include "macro.h"
#include "typedefs.h"

#include "app/framework/include/af.h" 
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "halDelay.h"
#include "Uart_Transmitter.h"
#include "Uart_CmdEnum.h"
#include "serial/serial.h"
#include "ZigbeeEvent.h"
#include "Uart_CmdExec.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_UartConectionError_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC uart connection Error \n\r");
    emberSerialPrintf(APP_SERIAL,"		Error Code: %u \n\r",data[1]);
    emberSerialPrintf(APP_SERIAL,"		seqNumber: %u \n\r",data[2]);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug
    return UartCmdStatus;
}

/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_FormNetwork_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u PanId;
    int8u Channel;
    int8u SeqNumber = data[0];

    PanId = MERGE16(data[2], data[3]);
    Channel = data[4];
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Form Network \n\r");
    emberSerialPrintf(APP_SERIAL,"		Pan ID: 0x%2x \n\r",PanId);
    emberSerialPrintf(APP_SERIAL,"		Channel: %u \n\r",Channel);
    emberSerialPrintf(APP_SERIAL,"		Forming.......",Channel);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug
    if((Channel < 10) && (Channel > 25)){
    #ifdef Debug
        emberSerialPrintf(APP_SERIAL,"		Channel Error",Channel);   
    #endif //Debug
//        UartSendUartError(CmdDataError, SeqNumber);
    }
    
    // store event data
    NwkFormEventData.PanId = PanId;
    NwkFormEventData.Channel = Channel;
    //call form event
    emberEventControlSetActive(NwkFormEventControl);
    return UartCmdStatus;
}

/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_GetNetworkInfo_Task(void) {
    int8u UartCmdStatus = UartCmdNormal;

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Nwk Info \n\r");
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug
    EmberNetworkParameters networkParams;
    EmberNodeType nodeType = EMBER_COORDINATOR;
    (void) emberAfGetNetworkParameters(&nodeType, &networkParams);
    SwapEndiannessEUI64(networkParams.extendedPanId);
    UartSendNwkInfo(networkParams.extendedPanId, networkParams.panId,
            networkParams.radioChannel);
    return UartCmdStatus;

}
/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_NetworkPjoin_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int8u Times;

    Times = data[2];
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Network Pjoin \n\r");
    emberSerialPrintf(APP_SERIAL,"		Times: 0x%x \n\r",Times);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug
    (void) emberAfPermitJoin(Times, 1);
    UartSendPjoinStatus(PermitJoin);
    return UartCmdStatus;

}
/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_PjoinStatus_Task(void) {
    int8u UartCmdStatus = UartCmdNormal;
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Pjoin Status \n\r");
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    UartSendPjoinStatus((u8_t) emberGetPermitJoining());
    return UartCmdStatus;

}
/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_ResetDevice_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u TimeDelay;

    NwkAddr = MERGE16(data[2], data[3]);
    TimeDelay = data[4];

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Reset \n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkAddr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		TimeDelay: 0x%x \n\r",TimeDelay);

    emberSerialPrintf(APP_SERIAL,"\n\r");

    #endif //Debug	
    return UartCmdStatus;

}
/**
 * @func   HC_UartConectionError_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
int8u HC_GetDeviceMCVersion_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;

    NwkAddr = MERGE16(data[2], data[3]);
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Device MC Version \n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkAddr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    return UartCmdStatus;

}

/**
 * @func   HC_ZclCLusterCmdResquest_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

int8u HC_ZclClusterCmdRequest_Task(int8u *data){
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr = MERGE16(data[0],data[1]);
    int8u Endpoint = data[2];
    int16u ClusterId = MERGE16(data[3],data[4]);
    int8u CmdPayloadLength = data[5];
    int8u ClusterCmdId = data[6];
    int8u* CmdPayload = &data[7];
    
    emberAfFillExternalBuffer((ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER | ZCL_DISABLE_DEFAULT_RESPONSE_MASK),
                        ClusterId, 
                        ClusterCmdId, 
                        "b",
                        &CmdPayload[7],
                        CmdPayloadLength - 1);
    emberAfSetCommandEndpoints(1, Endpoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, NwkAddr);                            
    return UartCmdStatus;
}

/**
 * @func   HC_ZclCLusterCmdResquest_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

int8u HC_ZclGlobalCmdRequest_Task(int8u *data){
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr = MERGE16(data[0],data[1]);
    int8u Endpoint = data[2];
    int16u ClusterId = MERGE16(data[3],data[4]);
    int8u GeneralCmdId = data[5];
    int8u CmdPayloadLength = data[6];
    int8u* CmdPayload = &data[7];
    
    emberAfFillExternalBuffer((ZCL_GLOBAL_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER),
                        ClusterId, 
                        GeneralCmdId, 
                        "b",
                        &CmdPayload[7],
                        CmdPayloadLength);
    emberAfSetCommandEndpoints(1, Endpoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, NwkAddr);                            
    return UartCmdStatus;
}

/**
 * @func   HC_ZclCLusterCmdResquest_Task
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

int8u HC_ZdoCmdRequest_Task(int8u *data){
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr = MERGE16(data[0],data[1]);
    int16u ZdoCmd = MERGE16 (data[2],data[3]);
    int8u CmdPayloadLength = data[6];
    int8u* CmdPayload = &data[7];
    switch(ZdoCmd){
        case NETWORK_ADDRESS_REQUEST:
        
        break;
        case IEEE_ADDRESS_REQUEST:
        break;
        case NODE_DESCRIPTOR_REQUEST:
        break;
        case POWER_DESCRIPTOR_REQUEST:
        break;
        case SIMPLE_DESCRIPTOR_REQUEST:
        break;
        case ACTIVE_ENDPOINTS_REQUEST:
        break;
        case MATCH_DESCRIPTORS_REQUEST:
        break;
        case DISCOVERY_CACHE_REQUEST:
        break;
        case END_DEVICE_ANNOUNCE:
        break;
        case SYSTEM_SERVER_DISCOVERY_REQUEST:
        break;
        case PARENT_ANNOUNCE:
        break;
        case FIND_NODE_CACHE_REQUEST:
        break;
        case END_DEVICE_BIND_REQUEST:
        break;
        case BIND_REQUEST:
        break;
        case UNBIND_REQUEST:
        break;
        case LQI_TABLE_REQUEST:
        break;
        case ROUTING_TABLE_REQUEST:
        break;
        case BINDING_TABLE_REQUEST:
        break;
        case LEAVE_REQUEST:
        break;
        case PERMIT_JOINING_REQUEST:
        break;
        case NWK_UPDATE_REQUEST:
        break;
        case COMPLEX_DESCRIPTOR_REQUEST:
        break;
        case USER_DESCRIPTOR_REQUEST:
        break;
        case USER_DESCRIPTOR_SET:
        break;
        case NETWORK_DISCOVERY_REQUEST:
        break;
        case DIRECT_JOIN_REQUEST:
        break;
        default:
        break;
        
    }
                           
    return UartCmdStatus;
}







//---------------------------------End of file -------------------------------//	
