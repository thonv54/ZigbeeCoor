
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

//------------------------HC_UartConectionError-------------------//
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

//----------------------------------------------------------------//	

//------------------------HC_FormNetwork-------------------//
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

//----------------------------------------------------------------//	

//------------------------HC_GetNetworkInfo-------------------//
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

//----------------------------------------------------------------//	

//------------------------HC_NetworkPjoin-------------------//
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

//----------------------------------------------------------------//	

//------------------------HC_LeaveNetwork-------------------//
int8u HC_LeaveNetwork_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u Rejoin;
    int8u RemoveChildren;

    NwkAddr = MERGE16(data[2], data[3]);
    Rejoin = data[4];
    RemoveChildren = data[5];
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Leave Zigbee Nwk \n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkID: 0x%2x \n\r", NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		Rejoin: %x \n\r",Rejoin);
    emberSerialPrintf(APP_SERIAL,"		RemoveChildren: %x \n\r",RemoveChildren);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug

    EmberEUI64 nullEui64 = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t options = 0;
    if (Rejoin == 1) {
        options |= EMBER_ZIGBEE_LEAVE_AND_REJOIN;
    }
    if (RemoveChildren == 1) {
        options |= EMBER_ZIGBEE_LEAVE_AND_REMOVE_CHILDREN;
    }

    (void) emberLeaveRequest(NwkAddr, nullEui64, options,
            EMBER_AF_DEFAULT_APS_OPTIONS);
    
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_PjoinStatus-------------------//
int8u HC_PjoinStatus_Task(void) {
    int8u UartCmdStatus = UartCmdNormal;
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Pjoin Status \n\r");
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    UartSendPjoinStatus((u8_t) emberGetPermitJoining());
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_GetMacAddr-------------------//
void HC_GetMacServiceDiscoveryCallback(const EmberAfServiceDiscoveryResult* result){
    
    
    int16u wNwkAddr;
    EmberEUI64 byMac;
    wNwkAddr = result-> matchAddress;
    memcpy(byMac, (int8u*) result->responseData, sizeof(EmberEUI64));
    SwapEndiannessEUI64(byMac);
    UartSendDeviceMAC(wNwkAddr, byMac);
    
}

int8u HC_GetMacAddr_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;

    NwkAddr = MERGE16(data[2], data[3]);
    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Device Mac Address \n\r");
    emberSerialPrintf(APP_SERIAL,"		Nwk Addr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug
    emberAfFindIeeeAddress	(NwkAddr,HC_GetMacServiceDiscoveryCallback);	
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_GetActiveEndPoint_Task-------------------//
int8u HC_GetActiveEndpoint_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;

    NwkAddr = MERGE16(data[2], data[3]);

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Active EndPoint \n\r");
    emberSerialPrintf(APP_SERIAL,"		Nwk Addr: 0x%2x \n\r", NwkAddr);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    (void)emberActiveEndpointsRequest(NwkAddr,EMBER_AF_DEFAULT_APS_OPTIONS);
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_GetSimpleDesCription-------------------//
int8u HC_GetSimpleDesCription_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u EndPoint;

    NwkAddr = MERGE16(data[2], data[3]);
    EndPoint = data[4];

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Simple Description \n\r");
    emberSerialPrintf(APP_SERIAL,"		Nwk Addr: 0x%2x \n\r", NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		EndPoint: 0x%x \n\r", EndPoint);
    emberSerialPrintf(APP_SERIAL,"    \n\r");
    #endif //Debug	
    (void)emberSimpleDescriptorRequest(NwkAddr,
                                       EndPoint,
                                       EMBER_AF_DEFAULT_APS_OPTIONS);
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_GetNodeDescription-------------------//
int8u HC_GetNodeDescription_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;

    NwkAddr = MERGE16(data[2], data[3]);

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Node Decription \n\r");
    emberSerialPrintf(APP_SERIAL,"		Nwk Addr: 0x%2x \n\r", NwkAddr);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    (void)emberNodeDescriptorRequest(NwkAddr,
                                              EMBER_AF_DEFAULT_APS_OPTIONS);
    return UartCmdStatus;

}

//----------------------------------------------------------------//

//------------------------HC_GetDeviceAttributeInfo-------------------//
int8u HC_GetDeviceAttributeInfo_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u EndPoint;
    int16u ClusterID;
    int16u AttributeID;

    NwkAddr = MERGE16(data[2], data[3]);
    EndPoint = data[4];
    ClusterID = MERGE16(data[5], data[6]);
    AttributeID = MERGE16(data[7], data[8]);

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Get Device Attribute Info \n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkAddr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		EndPoint: %u \n\r",EndPoint);
    emberSerialPrintf(APP_SERIAL,"		ClusterID: 0x%2x \n\r",ClusterID);
    emberSerialPrintf(APP_SERIAL,"		AttributeID: 0x%2x \n\r",AttributeID);
    emberSerialPrintf(APP_SERIAL,"\n\r");
    #endif //Debug	
    emberAfFillCommandGlobalClientToServerReadAttributes(ClusterID,
                                                             &AttributeID,
                                                             sizeof(AttributeID));
    emberAfSetCommandEndpoints(1,EndPoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, NwkAddr);
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_DeviceControl-------------------//
int8u HC_DeviceControl_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u EndPoint;
    int16u ClusterID;
    int8u ClusterCmdLength;
    int8u ClusterCmd[255];

    NwkAddr = MERGE16(data[2], data[3]);
    EndPoint = data[4];
    ClusterID = MERGE16(data[5], data[6]);
    ClusterCmdLength = data[7];
    (void) MEMCOPY(ClusterCmd, (int8u *) &data[8], ClusterCmdLength);

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Device Control\n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkAddr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		EndPoint: %u \n\r",EndPoint);
    emberSerialPrintf(APP_SERIAL,"		ClusterID: 0x%2x \n\r",ClusterID);
    emberSerialPrintf(APP_SERIAL,"		ClusterCmdLength: %u \n\r",ClusterCmdLength);
    emberSerialPrintf(APP_SERIAL,"		ClusterCmd : ");
    for(int8u i=0;i<ClusterCmdLength;i++) {
        emberSerialPrintf(APP_SERIAL,"%x ",ClusterCmd[i]);
    }
    emberSerialPrintf(APP_SERIAL,"\n\r");

    #endif //Debug
    int8u ClusterCmdId = ClusterCmd[0];
    ClusterCmdLength --;
    emberAfFillExternalBuffer((ZCL_CLUSTER_SPECIFIC_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER),
                            ClusterID,
                            ClusterCmdId,
                            "b",
                            &ClusterCmd[1],
                            ClusterCmdLength);
    emberAfSetCommandEndpoints(1,EndPoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, NwkAddr);                                           
    
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_WriteDeviceAtribute-------------------//
int8u HC_WriteDeviceAtribute_Task(int8u *data) {
    int8u UartCmdStatus = UartCmdNormal;
    int16u NwkAddr;
    int8u EndPoint;
    int16u ClusterID;
    int16u AttributeID;
    int8u DataType;
    int8u ValueLength;
    int8u Value[255];

    NwkAddr = MERGE16(data[2], data[3]);
    EndPoint = data[4];
    ClusterID = MERGE16(data[5], data[6]);
    AttributeID = MERGE16(data[7], data[8]);
    DataType = data[9];
    ValueLength = data[10];
    (void) MEMCOPY(Value, (int8u *) &data[11], ValueLength);

    #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Write Attribute\n\r");
    emberSerialPrintf(APP_SERIAL,"		NwkAddr: 0x%2x \n\r",NwkAddr);
    emberSerialPrintf(APP_SERIAL,"		EndPoint: %u \n\r",EndPoint);
    emberSerialPrintf(APP_SERIAL,"		ClusterID: 0x%2x \n\r",ClusterID);
    emberSerialPrintf(APP_SERIAL,"		AttributeID: 0x%2x \n\r",AttributeID);
    emberSerialPrintf(APP_SERIAL,"		DataType: 0x%x \n\r",DataType);
    emberSerialPrintf(APP_SERIAL,"		ValueLength: %u \n\r",ValueLength);

    emberSerialPrintf(APP_SERIAL,"		Value : ");
    for(int8u i=0;i<ValueLength;i++) {
        emberSerialPrintf(APP_SERIAL,"%x ",Value[i]);
    }
    emberSerialPrintf(APP_SERIAL,"\n\r");
    emberSerialPrintf(APP_SERIAL,"\n\r");

    #endif //Debug
    u8_t writeAttributeRecords[128];
    u8_t writeAttributeRecordsLen = sizeof(AttributeID) + sizeof(DataType) + ValueLength;
    memcpy(writeAttributeRecords,&AttributeID, sizeof(AttributeID));
    memcpy(&writeAttributeRecords[ sizeof(AttributeID) ],&DataType, sizeof(DataType));
    memcpy(&writeAttributeRecords[ sizeof(AttributeID) + sizeof(DataType)],Value, ValueLength);       
    emberAfFillCommandGlobalClientToServerWriteAttributes(ClusterID, 
                                                              writeAttributeRecords, 
                                                              writeAttributeRecordsLen);
    emberAfSetCommandEndpoints(1,EndPoint);
    emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, NwkAddr);      
    
    return UartCmdStatus;

}

//----------------------------------------------------------------//	

//------------------------HC_ResetDevice-------------------//
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

//----------------------------------------------------------------//	

//------------------------HC_GetDeviceMCVersion-------------------//
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

//----------------------------------------------------------------//	

//------------------------------HC Bind Device--------------------//	


int8u HC_BindUnbindDevice_Task(int8u *data){
    int8u UartCmdStatus = UartCmdNormal;
    int8u byBindUnbindCmd;
    int16u wNwkAddr1;
    int16u wNwkAddr2;
    int8u byEndpoint1;
    int8u byEndpoint2;
    int16u wClusterId;
    
    byBindUnbindCmd = data[2];
    wNwkAddr1 = MERGE16(data[3], data[4]);
    wNwkAddr2 = MERGE16(data[5], data[6]);
    byEndpoint1 = data[7];
    byEndpoint2 = data[8];
    wClusterId = MERGE16(data[9], data[10]);
    
    
        #ifdef Debug
    emberSerialPrintf(APP_SERIAL,"		HC Bind Device \n\r");
    if(byBindUnbindCmd == eBindCmd){
        emberSerialPrintf(APP_SERIAL,"		----BIND---  \n\r"); 
    }
    else if (byBindUnbindCmd == eUnbindCmd){
        emberSerialPrintf(APP_SERIAL,"		----UNBIND---  \n\r"); 
    }
    emberSerialPrintf(APP_SERIAL,"		NwkAddr1: 0x%2x \n\r",wNwkAddr1);
    emberSerialPrintf(APP_SERIAL,"		NwkAddr2: 0x%2x \n\r",wNwkAddr2);
    emberSerialPrintf(APP_SERIAL,"		Endpoint1: %u \n\r",byEndpoint1);
    emberSerialPrintf(APP_SERIAL,"		Endpoint2: %u \n\r",byEndpoint2);
    emberSerialPrintf(APP_SERIAL,"		ClusterId: 0x%2x \n\r",wClusterId);
    emberSerialPrintf(APP_SERIAL,"\n\r"); 
        #endif //Debug	// Debug
    
    

    if(byBindUnbindCmd == eBindCmd){
        BindDeviceEventData.NwkAddr1 = wNwkAddr1;
        BindDeviceEventData.NwkAddr2 = wNwkAddr2;
        BindDeviceEventData.Endpoint1 = byEndpoint1;
        BindDeviceEventData.Endpoint2 = byEndpoint2;
        BindDeviceEventData.ClusterId = wClusterId;
        emberEventControlSetActive(BindDeviceEventControl);
    }
    else if(byBindUnbindCmd == eUnbindCmd){
        UnbindDeviceEventData.NwkAddr1 = wNwkAddr1;
        UnbindDeviceEventData.NwkAddr2 = wNwkAddr2;
        UnbindDeviceEventData.Endpoint1 = byEndpoint1;
        UnbindDeviceEventData.Endpoint2 = byEndpoint2;
        UnbindDeviceEventData.ClusterId = wClusterId;
        emberEventControlSetActive(UnbindDeviceEventControl);
    }
    return UartCmdStatus;
}
