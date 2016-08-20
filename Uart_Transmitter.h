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


#ifndef __UART_TRANSMITTER_H__
#define __UART_TRANSMITTER_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "af.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    Zbs_UartError       	      =	  0x00,
    Zbs_NwkInfo                 =	  0x01,
    Zbs_LeaveNwkInfo            =   0x04,
    Zbs_PjoinStatus	            =	  0x05,
    Zbs_DeviceJoinNwk           =   0x06,
    Zbs_DeviceMac           	  =	  0x07,
    Zbs_ActiveEndpoint          = 	0x08,
    Zbs_SimpleDescription       =   0x09,
    Zbs_NodeDescription         =   0x0A,
    Zbs_DeviceAttributeInfo		  =	  0x0B,
    Zbs_DeviceMCVersion	        =	  0x0F,
    Zbs_BindUnbindDevice        =   0x10,
}Zbs_CommandID_enum;

typedef enum{
    eBindCmd    = 0,
    eUnbindCmd  = 1,
}BindUnbindCmd_enum;

//----------0x00-ZigbeeErrorCode----------//
typedef enum {
    UartCmdNormal     = 0,
    XorError          = 1,
    CmdIdNotAvaiable  = 2,
    CmdDataError      = 3,
    ZigBeeBusy        = 4,
}ZigbeeErrorCode_enum;

typedef enum{
    PermitJoin  =   1,
    NotPermitJoin=  0,
}PjoinState_enum;


typedef enum {
    CmdReady,
    CmdBusy,
}uartSendCommandStatus_enum;


typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u ErrorCode;
    int8u seqNumber;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_UartErrorDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u ExtendedPanId[8];
    int8u PanId[2];
    int8u Channel;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_NwkInfoDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u CheckXor;
    int8u TimeOut;
}Zbs_LeaveNwkInfoDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u State;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_PjoinStatusDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Mac[8];
    int8u CapInfo;
    int8u CheckXor;
    int8u TimeOut;    

}Zbs_DeviceJoinNwkDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Mac[8];
    int8u CheckXor;
    int8u TimeOut;
}Zbs_DeviceMacDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u ActiveEndpointCount;
    int8u* EndpointList;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_ActiveEndpointDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Endpoint;
    int8u ProfileId[2];
    int8u DeviceId[2];
    int8u InputClusterCount;
    int8u* InputClusterList;
    int8u OutputClusterCount;
    int8u* OutputClusterList;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_SimpleDescriptionDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u NodeDescription[13];
    int8u CheckXor;
    int8u TimeOut;
}Zbs_NodeDescriptionDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Endpoint;
    int8u ClusterId[2];
    int8u AttributeId[2];
    int8u DataType;
    int8u ValueLength;
    int8u* Value;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_DeviceAttributeInfoDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u CodeType;
    int8u CodeVersion[2];
    int8u CheckXor;
    int8u TimeOut;
}Zbs_DeviceMCVersionDataStr;
    
typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u BindUnbindCmd;
    int8u NwkAddr1[2];
    int8u NwkAddr2[2];
    int8u Endpoint1;
    int8u Endpoint2;
    int8u ClusterId[2];
    int8u Status;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_BindUnbindDeviceDataStr;


typedef struct{
    int8u EndpointNumber;
    int16u DeviceIdOfEndpoint;
}Zbs_EndpointInfo_str;

//---------------------Define------------------//
#define NORMAL_TIMEOUT	30				      //30ms
#define NormalTimeOut 	0xFF            // timeout off UART message send is NormalTimeOut*MinimumTimeOut
#define MinimumTimeOut  1000			      //MinimumTimeOut for lag value 1s


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   GetLastTxUartCmd
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
int16u GetLastTxUartCmd(void);
/**
 * @func   UartSendCommand
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendCommand(void);
/**
 * @func   testSendUartCmd
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void testSendUartCmd(void);
/**
 * @func   testPoint
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void testPoint(void);
/**
 * @func   UartSendUartError
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendUartError(int8u ErrorCode, int8u seqNumber);
/**
 * @func   UartSendNwkInfo
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendNwkInfo(int8u ExtendedPanId[8], int16u PanId, int8u Channel);
/**
 * @func   UartSendLeaveNwkInfo
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendLeaveNwkInfo(int16u NwkAddr);
/**
 * @func   UartSendPjoinStatus
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendPjoinStatus(int8u State);
/**
 * @func   UartSendDeviceJoinNwk
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendDeviceJoinNwk(int16u NwkAddr, EmberEUI64 Mac,int8u CapInfo);
/**
 * @func   UartSendDeviceMAC
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendDeviceMAC(int16u NwkAddr, EmberEUI64 Mac);
/**
 * @func   UartSendActiveEndpoint
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendActiveEndpoint(int16u NwkAddr,
                                  int8u ActiveEndpointCount,
                                  int8u* EndpointList);
/**
 * @func   UartSendSimpleDescription
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendSimpleDescription(int16u NwkAddr,
                               int8u Endpoint,
                               int16u ProfileId,
                               int16u DeviceId,
                               int8u InputClusterCount,
                               int8u* InputClusterList,
                               int8u OutputClusterCount,
                               int8u* OutputClusterList);
/**
 * @func   UartSendNodeDescription
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendNodeDescription(int16u NwkAddr,
                                  int8u NodeDescription[13]);
/**
 * @func   UartSendDeviceAttributeInfo
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendDeviceAttributeInfo(int16u NwkAddr,
                                  int8u Endpoint,
                                  int16u ClusterId,
                                  int16u AttributeId,
                                  int8u DataType,
                                  int8u ValueLength,
                                  int8u * Value);
/**
 * @func   UartSendBindUnbindDevice
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendBindUnbindDevice(int8u BindUnbindCmd,
                        int16u NwkAddr1,
                        int16u NwkAddr2,
                        int8u Endpoint1,
                        int8u Endpoint2,
                        int16u ClusterId,
                        int8u Status);
/**
 * @func   convertClustesIdsToTxBuff
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void convertClustesIdsToTxBuff(int8u* ClusterIds, int8u* Ret, int8u NumberOfClusterIds);



#endif






