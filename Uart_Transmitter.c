

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


//-----------------------------Khai bao thu vien---------------------//


#include "app/framework/include/af.h"
#include "Uart_CmdEnum.h"
#include "halDelay.h"
#include "macro.h"
#include "typedefs.h"
#include "macro.h"
#include "Uart_Transmitter.h"

//-----------------------------Khai bao bien-------------------------//


int8u UART_PACKET_KEY[2]	=	{0x4C,0x4D};

int8u UartTxCommandBuff[256];
#define MAX_RX_BUFFER  256
int16u UartTxCommandStartWaitTimer = 0;
int16u UartTxCommandCurrentTimeOut = 0;


int8u TxSeqNumber           =   0;




//---------------------------------UART Send Command--------------------------//

//------------------------------Send Uart Error Command----------------//
 /*
Mau ban tin:

CommandID			Error Code			seqNumber
1byte				1Byte               1 Byte
0x00   

Error Code: 
seqNumber :
*/


void UartSendUartError(int8u ErrorCode, int8u seqNumber){
	Zbs_UartErrorDataStr UartErrorData;
    UartErrorData.Length =  5   ;
    UartErrorData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    UartErrorData.CmdId = Zbs_UartError;
    UartErrorData.ErrorCode = ErrorCode;
    UartErrorData.seqNumber = seqNumber;
    UartErrorData.CheckXor = xorStr((unsigned char *) &UartErrorData.CmdId,(UartErrorData.Length -2));
    UartErrorData.TimeOut = NormalTimeOut;
        
    
    memcpy(&UartTxCommandBuff [GetLastTxUartCmd()], &UartErrorData,
			sizeof(UartErrorData));
    
}

void UartSendNwkInfo(int8u ExtendedPanId[8], int16u PanId, int8u Channel){
    Zbs_NwkInfoDataStr NwkInfoData;
    int16u val;
    
    NwkInfoData.Length = 14;
    NwkInfoData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    NwkInfoData.CmdId = Zbs_NwkInfo;
    memcpy(NwkInfoData.ExtendedPanId, ExtendedPanId, 8);
    val = SwapEndiannessInt16u(PanId);
    memcpy(NwkInfoData.PanId, (int8u*)&val, 2);
    NwkInfoData.Channel = Channel;
    NwkInfoData.CheckXor = xorStr((unsigned char *) &NwkInfoData.CmdId,(NwkInfoData.Length -2));
    NwkInfoData.TimeOut = NormalTimeOut;
    
    memcpy(&UartTxCommandBuff [GetLastTxUartCmd()], &NwkInfoData,
			sizeof(NwkInfoData));  
    
}

void UartSendLeaveNwkInfo(int16u NwkAddr){
    Zbs_LeaveNwkInfoDataStr LeaveNwkInfoData;
    int16u val;
    
    LeaveNwkInfoData.Length = 5;
    LeaveNwkInfoData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    LeaveNwkInfoData.CmdId = Zbs_LeaveNwkInfo;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(LeaveNwkInfoData.NwkAddr, (int8u*)&val, 2);

    LeaveNwkInfoData.CheckXor = xorStr((unsigned char *) &LeaveNwkInfoData.CmdId,(LeaveNwkInfoData.Length -2));
    LeaveNwkInfoData.TimeOut = NormalTimeOut;
    
    memcpy(&UartTxCommandBuff [GetLastTxUartCmd()], &LeaveNwkInfoData,
            sizeof(LeaveNwkInfoData));
    
}

void UartSendPjoinStatus(int8u State){
    Zbs_PjoinStatusDataStr PjoinStatusData;
    PjoinStatusData.Length = 4;
    PjoinStatusData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    PjoinStatusData.CmdId = Zbs_PjoinStatus;
    PjoinStatusData.State = State;
    PjoinStatusData.CheckXor = xorStr((unsigned char *) &PjoinStatusData.CmdId,(PjoinStatusData.Length -2));
    PjoinStatusData.TimeOut = NormalTimeOut;
    
    memcpy(&UartTxCommandBuff [GetLastTxUartCmd()], &PjoinStatusData,
            sizeof(PjoinStatusData));    
    
    
}


void UartSendDeviceJoinNwk(int16u NwkAddr, EmberEUI64 Mac,int8u CapInfo){
    
    Zbs_DeviceJoinNwkDataStr DeviceJoinNwkData;
    int16u StartIndexOfTxBuff;
    int16u val;
    
    
    DeviceJoinNwkData.Length = 14;
    DeviceJoinNwkData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    
    DeviceJoinNwkData.CmdId = Zbs_DeviceJoinNwk;
    
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(DeviceJoinNwkData.NwkAddr, (int8u*)&val, 2);
    
    memcpy(DeviceJoinNwkData.Mac, Mac, 8);
    
    DeviceJoinNwkData.CapInfo = CapInfo;
    
    DeviceJoinNwkData.CheckXor = xorStr((unsigned char *) &DeviceJoinNwkData.CmdId,14);
    DeviceJoinNwkData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &DeviceJoinNwkData,
            sizeof(DeviceJoinNwkData));
}

void UartSendDeviceMAC(int16u NwkAddr, EmberEUI64 Mac){
    
    Zbs_DeviceMacDataStr DeviceMacData;
    int16u StartIndexOfTxBuff;
    int16u val;
    
    
    DeviceMacData.Length = 13;
    DeviceMacData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    DeviceMacData.CmdId = Zbs_DeviceMac;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(DeviceMacData.NwkAddr, (int8u*)&val, 2);
    memcpy(DeviceMacData.Mac, Mac, 8);
    
    DeviceMacData.CheckXor = xorStr((unsigned char *) &DeviceMacData.CmdId,13);
    DeviceMacData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &DeviceMacData,
            sizeof(DeviceMacData));
}

void UartSendActiveEndpoint(int16u NwkAddr,
                                  int8u ActiveEndpointCount,
                                  int8u* EndpointList){
    Zbs_ActiveEndpointDataStr ActiveEndpointData;
    int16u StartIndexOfTxBuff;
    int16u val;
   
    
    ActiveEndpointData.Length = 6 + ActiveEndpointCount;
    ActiveEndpointData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    ActiveEndpointData.CmdId = Zbs_ActiveEndpoint;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(ActiveEndpointData.NwkAddr, (int8u*)&val, 2);
   
    ActiveEndpointData.ActiveEndpointCount = ActiveEndpointCount;
    ActiveEndpointData.EndpointList = EndpointList;
    
    ActiveEndpointData.CheckXor = xorStr((unsigned char*) &ActiveEndpointData.CmdId,4) ^ 
                                    xorStr((unsigned char*) ActiveEndpointData.EndpointList,ActiveEndpointCount);
    
    ActiveEndpointData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &ActiveEndpointData,
            6);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 6]  ,
            ActiveEndpointData.EndpointList,
            ActiveEndpointCount);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 6+ ActiveEndpointCount]  ,
            &ActiveEndpointData.CheckXor,
            2);
    
    
}


void UartSendSimpleDescription(int16u NwkAddr,
                               int8u Endpoint,
                               int16u ProfileId,
                               int16u DeviceId,
                               int8u InputClusterCount,
                               int8u* InputClusterList,
                               int8u OutputClusterCount,
                               int8u* OutputClusterList){
    Zbs_SimpleDescriptionDataStr SimpleDescriptionData;
    int16u StartIndexOfTxBuff;
    int16u val;
   
    
    SimpleDescriptionData.Length = 12 + 2* ( InputClusterCount + OutputClusterCount);
    SimpleDescriptionData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    SimpleDescriptionData.CmdId = Zbs_SimpleDescription;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(SimpleDescriptionData.NwkAddr, (int8u*)&val, 2);
   
    SimpleDescriptionData.Endpoint = Endpoint;
    
    val = SwapEndiannessInt16u(ProfileId);
    memcpy(SimpleDescriptionData.ProfileId, (int8u*)&val, 2);
    
    val = SwapEndiannessInt16u(DeviceId);
    memcpy(SimpleDescriptionData.DeviceId, (int8u*)&val, 2);
    
    SimpleDescriptionData.InputClusterCount = InputClusterCount;
    SimpleDescriptionData.InputClusterList = InputClusterList;
    SimpleDescriptionData.OutputClusterCount = OutputClusterCount;
    SimpleDescriptionData.OutputClusterList = OutputClusterList;
    
    SimpleDescriptionData.CheckXor = xorStr((unsigned char*) &SimpleDescriptionData.CmdId,8) ^ 
             SimpleDescriptionData.InputClusterCount^  
             xorStr((unsigned char*) SimpleDescriptionData.InputClusterList,(2* InputClusterCount))^
             SimpleDescriptionData.OutputClusterCount^
             xorStr((unsigned char*) SimpleDescriptionData.OutputClusterList,(2* OutputClusterCount));
    
    SimpleDescriptionData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &SimpleDescriptionData,
            10);
    
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 10]  ,
            &SimpleDescriptionData.InputClusterCount,
            1);
    
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 11]  ,
            SimpleDescriptionData.InputClusterList,
            2*InputClusterCount);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 11 + 2*InputClusterCount]  ,
            &SimpleDescriptionData.OutputClusterCount,
            1);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 12 + 2*OutputClusterCount]  ,
            SimpleDescriptionData.OutputClusterList,
            2*OutputClusterCount);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 12 + 2* (InputClusterCount +OutputClusterCount)]  ,
            &SimpleDescriptionData.CheckXor,
            2);

}

                     
void UartSendNodeDescription(int16u NwkAddr,
                                  int8u NodeDescription[13]){
    Zbs_NodeDescriptionDataStr NodeDescriptionData;
    int16u StartIndexOfTxBuff;
    int16u val;
   
    
    NodeDescriptionData.Length = 18;
    NodeDescriptionData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    NodeDescriptionData.CmdId = Zbs_NodeDescription;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(NodeDescriptionData.NwkAddr, (int8u*)&val, 2);
    
    memcpy(NodeDescriptionData.NodeDescription,NodeDescription,13);
    
    NodeDescriptionData.CheckXor = xorStr((unsigned char*) &NodeDescriptionData.CmdId,16);                       
    
    NodeDescriptionData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &NodeDescriptionData,
            sizeof(NodeDescriptionData));
}
                     
void UartSendDeviceAttributeInfo(int16u NwkAddr,
                                  int8u Endpoint,
                                  int16u ClusterId,
                                  int16u AttributeId,
                                  int8u DataType,
                                  int8u ValueLength,
                                  int8u * Value){
    Zbs_DeviceAttributeInfoDataStr DeviceAttributeInfo;
    int16u StartIndexOfTxBuff;
    int16u val;


    DeviceAttributeInfo.Length = 12  + ValueLength;
    DeviceAttributeInfo.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    DeviceAttributeInfo.CmdId = Zbs_DeviceAttributeInfo;
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(DeviceAttributeInfo.NwkAddr, (int8u*)&val, 2);
    DeviceAttributeInfo.Endpoint = Endpoint;
    val = SwapEndiannessInt16u(ClusterId);
    memcpy(DeviceAttributeInfo.ClusterId, (int8u*)&val, 2);
    val = SwapEndiannessInt16u(AttributeId);
    memcpy(DeviceAttributeInfo.AttributeId, (int8u*)&val, 2);
    DeviceAttributeInfo.DataType = DataType;
    DeviceAttributeInfo.ValueLength = ValueLength;
    DeviceAttributeInfo.Value = Value;
  
    
    DeviceAttributeInfo.CheckXor = xorStr((unsigned char*) &DeviceAttributeInfo.CmdId,10) ^ 
                  xorStr((unsigned char*) DeviceAttributeInfo.Value,DeviceAttributeInfo.ValueLength);
    
    DeviceAttributeInfo.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &DeviceAttributeInfo,
            12);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 12]  ,
            DeviceAttributeInfo.Value,
            DeviceAttributeInfo.ValueLength);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 12+ DeviceAttributeInfo.ValueLength]  ,
            &DeviceAttributeInfo.CheckXor,
            2);
}



void UartSendBindUnbindDevice(int8u BindUnbindCmd,
                        int16u NwkAddr1,
                        int16u NwkAddr2,
                        int8u Endpoint1,
                        int8u Endpoint2,
                        int16u ClusterId,
                        int8u Status){
    Zbs_BindUnbindDeviceDataStr BindUnbindDeviceData;
    int16u val;
    
    
    
    BindUnbindDeviceData.Length = 13;
    BindUnbindDeviceData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    BindUnbindDeviceData.CmdId = Zbs_BindUnbindDevice;
    BindUnbindDeviceData.BindUnbindCmd = BindUnbindCmd;
    val = SwapEndiannessInt16u(NwkAddr1);
    memcpy(BindUnbindDeviceData.NwkAddr1, (int8u*)&val, 2);
    val = SwapEndiannessInt16u(NwkAddr2);
    memcpy(BindUnbindDeviceData.NwkAddr2, (int8u*)&val, 2);
    BindUnbindDeviceData.Endpoint1 = Endpoint1;
    BindUnbindDeviceData.Endpoint2 = Endpoint2;
    val = SwapEndiannessInt16u(ClusterId);
    memcpy(BindUnbindDeviceData.ClusterId, (int8u*)&val, 2);
    BindUnbindDeviceData.Status = Status;

    memcpy(&UartTxCommandBuff [GetLastTxUartCmd()], &BindUnbindDeviceData,
            sizeof(BindUnbindDeviceData));      
}



//---------------------------------//----------------------------------//------------------------------//


void convertClustesIdsToTxBuff(int8u* ClusterIds, int8u* Ret, int8u NumberOfClusterIds){
    int16u val;
    unsigned char i;
    unsigned int tempint16u;
    for(i=0;i<NumberOfClusterIds;i++){
        tempint16u = MERGE16(ClusterIds [2*i +1], ClusterIds[2*i]);
        val = SwapEndiannessInt16u(tempint16u);
        memcpy((int8u*)&Ret[2*i],(int8u*)&val,2);
    }
}

void convertEndpointInfoToTxBuff(Zbs_EndpointInfo_str* EndpointInfo,int8u* Ret, int8u NumberOfEndPoint){

    int16u val;
    unsigned char i;
    for(i=0;i<NumberOfEndPoint;i++){
        memcpy((int8u*)&Ret[3*i],(int8u*)&EndpointInfo[i].EndpointNumber, 1);
        val = SwapEndiannessInt16u(EndpointInfo[i].DeviceIdOfEndpoint);
        memcpy((int8u*)&Ret[3*i +1],(int8u*)&val, 2);
    }

}


void testSendUartCmd(void){
    
    
    int8u Mac[8] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    
    Zbs_EndpointInfo_str EndpointInfo[2] = {{0x01,0x1234},{0x02,0x5678}};
    int8u InputClusterIds[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
    int8u OutputClusterIds[10] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19};
    int8u NodeDescription[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    int8u EndpointList[5] = {1,2,3,5,7};
    int8u Data = 0x23;
    int8u EndpointInfoDataToTxBuff[16];
    int8u InputClusterIdsDataToTxBuff[16];
    int8u OutputClusterIdsDataToTxBuff[16];
    
    convertEndpointInfoToTxBuff(EndpointInfo,EndpointInfoDataToTxBuff, 2);
    
//    convertClustesIdsToTxBuff(InputClusterIds, InputClusterIdsDataToTxBuff, 5);
//    convertClustesIdsToTxBuff(OutputClusterIds, OutputClusterIdsDataToTxBuff, 5);
    
    UartSendDeviceMAC(0x1234,Mac);
    UartSendActiveEndpoint(0x1234,5,EndpointList);
    UartSendSimpleDescription(0x1234,1,0x4567,0x89AB,5,InputClusterIds,5,OutputClusterIds);
    UartSendNodeDescription(0x1234, NodeDescription);
    UartSendDeviceJoinNwk(0x1234,Mac,0x55);
    UartSendPjoinStatus(0x01);
    UartSendLeaveNwkInfo(0x1234);
    UartSendNwkInfo (Mac,0x1234,15);
    UartSendDeviceAttributeInfo(0x1234,0x01,0x0104,0x0001,0x20,0x01,&Data);
}

void testPoint(void){
    int8u* PtrA;
    int8u* PtrB;
    int8u dataA[20] = "hello";
    int8u dataB[20] = "rocket";
//    memcpy(dataB,dataA,20);
    PtrA = dataA;
    PtrB = PtrA;
    memcpy (PtrA,dataB,20);
    emberSerialPrintf(APP_SERIAL," \n\r",PtrA);
    emberSerialPrintf(APP_SERIAL," DataA : %s",dataA);
    emberSerialPrintf(APP_SERIAL," DataB : %s",dataB);
    emberSerialPrintf(APP_SERIAL," PtrA : %s",PtrA);
    emberSerialPrintf(APP_SERIAL," PtrB : %s",PtrB);
    emberSerialPrintf(APP_SERIAL," \n\r",PtrA);
}


//----------------------------UART Send Command Utility-----------------------//

int16u GetLastTxUartCmd(void){
    int16u Head = 0;
    int16u Length = 0;
    while(UartTxCommandBuff[Head] != 0){
        if(UartTxCommandBuff[Head] != 0){
            Length = UartTxCommandBuff[Head];
            Head = Head+Length+2;
        }
    }
    return Head;
}


int8u CheckWaitTimeOut(void){

	if(UartTxCommandStartWaitTimer !=0 ){
		if(CommonGetDurationTime(UartTxCommandStartWaitTimer) >= UartTxCommandCurrentTimeOut){
			//clear timer
			UartTxCommandStartWaitTimer = 0;
			return CmdReady;
		}
		else{
			return CmdBusy;
		}
	}
	else{
		return CmdReady;
	}
}




//---------------------------------Check Uart Status And Send Quere Data Buffer-------------------------------//
void UartSendCommand(void){
	//	Check buff
	if(UartTxCommandBuff[0] != 0){
		//	Check Timeout


		switch (CheckWaitTimeOut()){
			case CmdBusy:
			break;
			case CmdReady:
				//	Send buff
				if(UartTxCommandBuff[UartTxCommandBuff[0]+1] == 
                   NormalTimeOut){
					UartTxCommandCurrentTimeOut = NORMAL_TIMEOUT;
				}
				else{
					UartTxCommandCurrentTimeOut = UartTxCommandBuff[UartTxCommandBuff[0]+1] * MinimumTimeOut;
				}
				emberSerialWriteData(HC_SERIAL,UART_PACKET_KEY,2);
				emberSerialWriteData(HC_SERIAL,UartTxCommandBuff, (UartTxCommandBuff[0]+1));
				memcpy(UartTxCommandBuff, &UartTxCommandBuff[UartTxCommandBuff[0]+2],
						(MAX_RX_BUFFER - (UartTxCommandBuff[0]+2)));
				UartTxCommandStartWaitTimer = (int16u)halCommonGetInt32uMillisecondTick();
			break;
			default:
			break;
		}
	}
}
//----------------------------------------------END--------------------------------------//






