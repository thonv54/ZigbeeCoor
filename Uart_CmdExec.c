
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
    int16u NwkAddr = MERGE16(data[2],data[3]);
    int8u Endpoint = data[4];
    int16u ClusterId = MERGE16(data[5],data[6]);
    int8u CmdPayloadLength = data[7];
    int8u* CmdPayload = &data[8];
    
    emberAfFillExternalBuffer((ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER | ZCL_DISABLE_DEFAULT_RESPONSE_MASK),
                        ClusterId,
                        CmdPayload[0],
                        "b",
                        &CmdPayload[1],
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
    int16u NwkAddr = MERGE16(data[2],data[3]);
    int8u Endpoint = data[4];
    int16u ClusterId = MERGE16(data[5],data[6]);
    int8u GeneralCmdId = data[7];
    int8u CmdPayloadLength = data[8];
    int8u* CmdPayload = &data[9];
    
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
    int16u ApsNwkAddr = MERGE16(data[2],data[3]);
    int16u ZdoCmd = MERGE16 (data[4],data[5]);
//    int8u CmdPayloadLength = data[6];
    int8u* CmdPayload = &data[7];
    
    if(ZdoCmd == NETWORK_ADDRESS_REQUEST){
//        int8u seqNumber = CmdPayload[0];
        
        int8u LongAddr[8];
        memcpy(LongAddr,(int8u*)&CmdPayload[1],sizeof(LongAddr));
        int8u ReportKids = CmdPayload[9];
        int8u ChildStartIndex = CmdPayload[10];
        emberNetworkAddressRequest(LongAddr,
                                    ReportKids,         // report kids?
                                    ChildStartIndex);            // child start index
    }
    else if(ZdoCmd == IEEE_ADDRESS_REQUEST){
//        int8u seqNumber = CmdPayload[0];
        
        int16u NwkAddr = MERGE16(CmdPayload[2],CmdPayload[1]);
        int8u ReportKids = CmdPayload[3];
        int8u ChildStartIndex = CmdPayload[4];
        emberIeeeAddressRequest(NwkAddr,
                                ReportKids,
                                ChildStartIndex,
                                EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if(ZdoCmd == NODE_DESCRIPTOR_REQUEST){
//        int8u seqNumber = CmdPayload[0];   
        
        int16u NwkAddr = MERGE16(CmdPayload[2],CmdPayload[1]);
        emberNodeDescriptorRequest (NwkAddr,
                                    EMBER_AF_DEFAULT_APS_OPTIONS);
        
    }
    
    else if(ZdoCmd == POWER_DESCRIPTOR_REQUEST){
    }
    else if (ZdoCmd == SIMPLE_DESCRIPTOR_REQUEST){
//        int8u seqNumber = CmdPayload[0];
        
        int16u NwkAddr = MERGE16(CmdPayload[2],CmdPayload[1]);
        int8u Endpoint = CmdPayload[3];

        emberSimpleDescriptorRequest(NwkAddr,
                                         Endpoint,
                                         EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if(ZdoCmd == ACTIVE_ENDPOINTS_REQUEST){
//        int8u seqNumber = CmdPayload[0];   
        
        int16u NwkAddr = MERGE16(CmdPayload[2],CmdPayload[1]);
        emberActiveEndpointsRequest (NwkAddr,
                                    EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if(ZdoCmd == MATCH_DESCRIPTORS_REQUEST){
    }
    else if(ZdoCmd == DISCOVERY_CACHE_REQUEST){
    }
    else if(ZdoCmd == END_DEVICE_ANNOUNCE){
    }
    else if(ZdoCmd == SYSTEM_SERVER_DISCOVERY_REQUEST){
    }
    else if(ZdoCmd == PARENT_ANNOUNCE){
    }
    else if(ZdoCmd == FIND_NODE_CACHE_REQUEST){
    }
    else if(ZdoCmd == END_DEVICE_BIND_REQUEST){
    }
    else if (ZdoCmd == BIND_REQUEST){
  /// Request:  <transaction sequence number: 1>
  ///           <source EUI64:8> <source endpoint:1> 
  ///           <cluster ID:2> <destination address:3 or 10>  
        
//        int8u seqNumber = CmdPayload[0];  
        
        int8u SourceEui[8];
        memcpy(SourceEui, (int8u*)&CmdPayload[1],sizeof(SourceEui));
        int8u SourceEndpoint = CmdPayload[9];
        int8u ClusterId = MERGE16(CmdPayload[10],CmdPayload[11]);
        int8u DestEui[8];
        memcpy(DestEui, (int8u*)&CmdPayload[12],sizeof(DestEui));
        int8u DestEndpoint = CmdPayload[20];
        
        emberBindRequest(ApsNwkAddr,          // who gets the bind req
                SourceEui,       // source eui IN the binding
                SourceEndpoint,
                ClusterId,       
                UNICAST_BINDING, // binding type
                DestEui,         // destination eui IN the binding
                0,               // groupId for new binding
                DestEndpoint,
                EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if (ZdoCmd == UNBIND_REQUEST){
//        int8u seqNumber = CmdPayload[0];  
        
        int8u SourceEui[8];
        memcpy(SourceEui, (int8u*)&CmdPayload[1],sizeof(SourceEui));
        int8u SourceEndpoint = CmdPayload[9];
        int8u ClusterId = MERGE16(CmdPayload[10],CmdPayload[11]);
        int8u DestEui[8];
        memcpy(DestEui, (int8u*)&CmdPayload[12],sizeof(DestEui));
        int8u DestEndpoint = CmdPayload[20];
        
        emberUnbindRequest(ApsNwkAddr,          // who gets the bind req
                SourceEui,       // source eui IN the binding
                SourceEndpoint,
                ClusterId,       
                UNICAST_BINDING, // binding type
                DestEui,         // destination eui IN the binding
                0,               // groupId for new binding
                DestEndpoint,
                EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if (ZdoCmd == LQI_TABLE_REQUEST){
    }
    else if(ZdoCmd == ROUTING_TABLE_REQUEST){
    }
    else if (ZdoCmd == BINDING_TABLE_REQUEST){
    }
    else if (ZdoCmd == LEAVE_REQUEST){
        /// Request:  <transaction sequence number: 1> <EUI64:8> <flags:1>
        ///          The flag bits are:
        ///          0x40 remove children
        ///          0x80 rejoin
        EmberNodeId target = ApsNwkAddr;
        bool removeChildren = (bool)((CmdPayload[9]>>6) & 0x01);
        bool rejoin = (bool)((CmdPayload[9]>>7) & 0x01);
        EmberEUI64 nullEui64 = { 0, 0, 0, 0, 0, 0, 0, 0 };
        EmberStatus status;

        uint8_t options = 0;
        if (rejoin) {
        options |= EMBER_ZIGBEE_LEAVE_AND_REJOIN;
        }
        if (removeChildren) {
        options |= EMBER_ZIGBEE_LEAVE_AND_REMOVE_CHILDREN;
        }

        status = emberLeaveRequest(target,
                                 nullEui64,
                                 options,
                                 EMBER_AF_DEFAULT_APS_OPTIONS);
    }
    else if (ZdoCmd == PERMIT_JOINING_REQUEST){
    }
    else if (ZdoCmd == NWK_UPDATE_REQUEST){
    }
    else if (ZdoCmd == COMPLEX_DESCRIPTOR_REQUEST){
    }
    else if (ZdoCmd == USER_DESCRIPTOR_REQUEST){
    }
    else if (ZdoCmd == USER_DESCRIPTOR_SET){
    }
    else if (ZdoCmd == NETWORK_DISCOVERY_REQUEST){
    }
    else if (ZdoCmd == DIRECT_JOIN_REQUEST){
    }

    return UartCmdStatus;
}







//---------------------------------End of file -------------------------------//	
