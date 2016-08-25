

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

/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
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
/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
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
/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
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




/**
 * @func   NwkFormEventFunction
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendZclClusterCmdResponse(int16u NwkAddr,
                                   int8u Endpoint,
                                   int16u ClusterId,
                                   int8u CmdPayloadLength,
                                   int8u *CmdPayload){
                                       
    Zbs_ZclClusterCmdResponseStr ZclClusterCmdResponseData;
    int16u StartIndexOfTxBuff;
    int16u val;
    
    ZclClusterCmdResponseData.Length = CmdPayloadLength + 9;
    ZclClusterCmdResponseData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    ZclClusterCmdResponseData.CmdId = Zbs_ZclClusterCmdResponse;
    
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(ZclClusterCmdResponseData.NwkAddr, (int8u*)&val, 2);
    
    ZclClusterCmdResponseData.Endpoint = Endpoint;
    
    val = SwapEndiannessInt16u(ClusterId);
    memcpy(ZclClusterCmdResponseData.ClusterId, (int8u*)&val, 2);
    
    ZclClusterCmdResponseData.CmdPayloadLength = CmdPayloadLength;
    
    ZclClusterCmdResponseData.CmdPayload = CmdPayload;
    
    ZclClusterCmdResponseData.CheckXor = 
        xorStr((unsigned char *) &ZclClusterCmdResponseData.CmdId,7) ^
        xorStr(ZclClusterCmdResponseData.CmdPayload,ZclClusterCmdResponseData.CmdPayloadLength);

                                
    
    ZclClusterCmdResponseData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &ZclClusterCmdResponseData,
            9);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 9]  ,
            ZclClusterCmdResponseData.CmdPayload,
            ZclClusterCmdResponseData.CmdPayloadLength);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 9 + ZclClusterCmdResponseData.CmdPayloadLength]  ,
            &ZclClusterCmdResponseData.CheckXor,
            2);
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
void UartSendZclGlobalCmdResponse(int16u NwkAddr,
                                   int8u Endpoint,
                                   int16u ClusterId,
                                   int8u GeneralCmd,
                                   int8u CmdPayloadLength,
                                   int8u *CmdPayload){
    Zbs_ZclGlobalCmdResponseStr ZclGlobalCmdResponseData;
    int16u StartIndexOfTxBuff;
    int16u val;
    
    ZclGlobalCmdResponseData.Length = CmdPayloadLength + 10;
    ZclGlobalCmdResponseData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    ZclGlobalCmdResponseData.CmdId = Zbs_ZclGlobalCmdResponse;
    
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(ZclGlobalCmdResponseData.NwkAddr, (int8u*)&val, 2);
    
    ZclGlobalCmdResponseData.Endpoint = Endpoint;
    
    val = SwapEndiannessInt16u(ClusterId);
    memcpy(ZclGlobalCmdResponseData.ClusterId, (int8u*)&val, 2);
    
    ZclGlobalCmdResponseData.GeneralCmd = GeneralCmd;
    
    ZclGlobalCmdResponseData.CmdPayloadLength = CmdPayloadLength;
    
    ZclGlobalCmdResponseData.CmdPayload = CmdPayload;
    
    ZclGlobalCmdResponseData.CheckXor = 
        xorStr((unsigned char *) &ZclGlobalCmdResponseData.CmdId,8) ^
        xorStr(ZclGlobalCmdResponseData.CmdPayload, ZclGlobalCmdResponseData.CmdPayloadLength);
    
    ZclGlobalCmdResponseData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &ZclGlobalCmdResponseData,
            10);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 10]  ,
            ZclGlobalCmdResponseData.CmdPayload,
            ZclGlobalCmdResponseData.CmdPayloadLength);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 10+ ZclGlobalCmdResponseData.CmdPayloadLength]  ,
            &ZclGlobalCmdResponseData.CheckXor,
            2);
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
void UartSendZdoCmdResponse(int16u NwkAddr,
                            int16u ZdoCmd,
                            int8u CmdPayloadLength,
                            int8u *CmdPayload){
    Zbs_ZdoCmdResponseStr ZdoCmdResponseData;
    int16u StartIndexOfTxBuff;
    int16u val;
    
    ZdoCmdResponseData.Length = CmdPayloadLength + 8;
    ZdoCmdResponseData.TxSeqNumber = TxSeqNumber;
    TxSeqNumber ++;
    ZdoCmdResponseData.CmdId = Zbs_ZdoCmdResponse;
    
    val = SwapEndiannessInt16u(NwkAddr);
    memcpy(ZdoCmdResponseData.NwkAddr, (int8u*)&val, 2);
    
    val = SwapEndiannessInt16u(ZdoCmd);
    memcpy(ZdoCmdResponseData.ZdoCmd, (int8u*)&val, 2);
    
    ZdoCmdResponseData.CmdPayloadLength = CmdPayloadLength;
    
    ZdoCmdResponseData.CmdPayload = CmdPayload;
    
    ZdoCmdResponseData.CheckXor = 
        xorStr((unsigned char *) &ZdoCmdResponseData.CmdId,6) ^
        xorStr(ZdoCmdResponseData.CmdPayload,ZdoCmdResponseData.CmdPayloadLength);

    ZdoCmdResponseData.TimeOut = NormalTimeOut;
    
    StartIndexOfTxBuff = GetLastTxUartCmd();
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff], 
            &ZdoCmdResponseData,
            8);
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff + 8]  ,
            ZdoCmdResponseData.CmdPayload,
            ZdoCmdResponseData.CmdPayloadLength);  
    memcpy(&UartTxCommandBuff [StartIndexOfTxBuff+ 8+ ZdoCmdResponseData.CmdPayloadLength]  ,
            &ZdoCmdResponseData.CheckXor,
            2);
}

//---------------------------------//----------------------------------//------------------------------//

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






